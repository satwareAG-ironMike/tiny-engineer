#!/usr/bin/env bash
# Gate oracles for SOD 2026-08-29 + OSINT fork audit (tiny-engineer).
# Each subcommand asserts everything, exits 1 on failure, prints its token only on success.
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OPS_FILE="$HOME/.agent-state/daily-ops/satwareAG-ironMike/tiny-engineer/2026-08-29.md"
UPSTREAM_REPO="jamro/tiny-engineer"
FORKS=(theInternauts phillyjack praveenvsn johan-- pcannon67)
REPORT_FILE="$REPO_DIR/docs/research/tiny-engineer-osint-2026-08-29.md"
PROTECTED_REPO="satwareAG-ironMike/tiny-engineer"
WORK_BRANCH="docs/fork-state"

show_help() {
  cat <<'EOF'
gate-check-sod.sh SUBCOMMAND - SOD/OSINT gate oracles (2026-08-29)

Subcommands:
  drift     main is the exact upstream mirror (main...jamro/main 0/0, branch-independent)
  forks     no non-satware fork ahead of upstream; runs selftest (negative control) first
  selftest  negative control: parser must reject ahead_by=1, accept ahead_by=0
  report    daily-ops file contains all required OSINT report sections
   sod       daily-ops file contains SOD state under the sod marker
   issue     issue #1 closed per API, fix commit in objects, both recorded in report
   protect   exactly one active branch ruleset on main, deletion/force_push/non_fast_forward enabled
   branch    working branch on origin with report committed; main still exact mirror
   verdict   final verdict + infra actions recorded in daily-ops file

Exit 0 + token on success; exit 1 + FAIL message on failure.
EOF
}

fail() { echo "FAIL: $*" >&2; exit 1; }

# compare-API JSON -> success iff ahead_by == 0
parse_compare() {
  local ahead
  ahead="$(jq -r '.ahead_by' <<<"$1")" || return 1
  [[ "$ahead" == "0" ]]
}

cmd_selftest() {
  local positive='{"status":"ahead","ahead_by":1,"behind_by":0}'
  local negative='{"status":"behind","ahead_by":0,"behind_by":2}'
  if parse_compare "$positive"; then
    fail "negative control: parser accepted ahead_by=1"
  fi
  parse_compare "$negative" || fail "negative control: parser rejected ahead_by=0"
  echo "selftest OK"
}

cmd_drift() {
  # Invariant: the main branch is an exact mirror of upstream jamro/main.
  # Anchored on the main ref (not HEAD) so it is branch-independent and stays
  # green while work lands on feature branches; the per-branch HEAD/origin sync
  # is a separate concern (see the GIT-CLEAN-IN-SYNC gate, not this one).
  local counts expected
  counts="$(git -C "$REPO_DIR" rev-list --left-right --count main...jamro/main)" \
    || fail "rev-list failed (fetch jamro/* refs first)"
  expected="$(printf '0\t0')"
  [[ "$counts" == "$expected" ]] || fail "main drifts from upstream: $counts"
  echo "drift OK"
}

cmd_forks() {
  cmd_selftest >/dev/null || fail "selftest failed before live fork check"
  local f json ahead
  for f in "${FORKS[@]}"; do
    json="$(gh api "repos/$UPSTREAM_REPO/compare/jamro:main...$f:main")" \
      || fail "compare API failed for $f"
    if ! parse_compare "$json"; then
      ahead="$(jq -r '.ahead_by' <<<"$json")"
      fail "fork $f is AHEAD of upstream (ahead_by=$ahead) - inspect its unique commits"
    fi
  done
  echo "forks OK"
}

cmd_report() {
  [[ -f "$OPS_FILE" ]] || fail "missing $OPS_FILE"
  local section
  for section in "## Upstream" "## Forks" "## Maintainer" "## Issues & PRs" "## Verdict"; do
    grep -qF "$section" "$OPS_FILE" || fail "report missing section: $section"
  done
  echo "report OK"
}

cmd_sod() {
  [[ -f "$OPS_FILE" ]] || fail "missing $OPS_FILE"
  grep -qF '<!-- sod:2026-08-29 -->' "$OPS_FILE" || fail "missing sod marker"
  grep -qF 'Top-3' "$OPS_FILE" || fail "missing Top-3 priorities"
  grep -qF 'Coverage' "$OPS_FILE" || fail "missing Coverage"
  grep -qF 'Skills loaded' "$OPS_FILE" || fail "missing Skills loaded"
  echo "sod OK"
}

cmd_issue() {
  local state
  [[ -f "$OPS_FILE" ]] || fail "missing $OPS_FILE"
  state="$(gh api "repos/$UPSTREAM_REPO/issues/1" --jq '.state')" || fail "issue API failed"
  [[ "$state" == "closed" ]] || fail "issue #1 state=$state (expected closed)"
  git -C "$REPO_DIR" cat-file -e '047ce22^{commit}' 2>/dev/null \
    || fail "fix commit 047ce22 not in local objects"
  grep -qF 'BB-BenBridges' "$OPS_FILE" || fail "report missing reporter"
  grep -qF '047ce22' "$OPS_FILE" || fail "report missing fix commit"
  echo "issue OK"
}

cmd_osint_report() {
  [[ -f "$REPORT_FILE" ]] || fail "missing $REPORT_FILE"
  local section
  for section in "## 1. Project profile" "## 2. Technical footprint" "## 3. Timeline" \
    "## 4. Web presence" "## 5. Creator profile" "## 6. Name disambiguation" \
    "## 7. Discrepancies" "## 8. Verdict"; do
    grep -qF "$section" "$REPORT_FILE" || fail "report missing section: $section"
  done
  echo "osint OK"
}

cmd_confidence() {
  [[ -f "$REPORT_FILE" ]] || fail "missing $REPORT_FILE"
  local n
  n="$(grep -oE 'HIGH|MEDIUM|LOW|UNVERIFIED' "$REPORT_FILE" | wc -l)"
  [[ "$n" -ge 15 ]] || fail "only $n confidence marks (need >= 15)"
  echo "confidence OK"
}

cmd_identity() {
  [[ -f "$REPORT_FILE" ]] || fail "missing $REPORT_FILE"
  local id present=0
  for id in "jamro" "kjamroz83@gmail.com" "jamropl" "jamroification" "jamro.net" "UCJvyD5c7kj6Izt_mL31Om0g"; do
    grep -qF "$id" "$REPORT_FILE" && present=$((present + 1))
  done
  [[ "$present" -ge 5 ]] || fail "only $present/6 identity identifiers present (need >= 5)"
  echo "identity OK"
}

cmd_deep() {
  local hits state created views
  hits="$(curl -s -m 20 'https://hn.algolia.com/api/v1/search?query=%22tiny%20engineer%22&tags=story' | jq '.hits | length')" \
    || fail "HN Algolia query failed"
  [[ "$hits" == "0" ]] || fail "HN now has $hits stories (report says 0)"
  state="$(gh api "repos/$UPSTREAM_REPO/issues/1" --jq '.state')" || fail "issue API failed"
  [[ "$state" == "closed" ]] || fail "issue #1 state=$state"
  created="$(gh api "repos/$UPSTREAM_REPO" --jq '.created_at')" || fail "repo API failed"
  [[ "$created" == 2026-08-21* ]] || fail "repo created_at=$created (expected 2026-08-21)"
  views="$(curl -s -m 20 -A 'Mozilla/5.0 (X11; Linux x86_64)' 'https://www.youtube.com/watch?v=RX_QRxdXMjg' | grep -oE '"viewCount":"[0-9]+"' | head -1 | grep -oE '[0-9]+')" \
    || fail "YouTube fetch failed"
  [[ -n "$views" && "$views" -gt 0 ]] || fail "no video viewCount found"
  echo "deep OK"
}

cmd_ops_ref() {
  [[ -f "$OPS_FILE" ]] || fail "missing $OPS_FILE"
  grep -qF 'docs/research/tiny-engineer-osint-2026-08-29.md' "$OPS_FILE" \
    || fail "daily-ops file does not reference the full report path"
  echo "ops-ref OK"
}

cmd_protect() {
  # Rulesets API: the list endpoint omits conditions/rules (summary only);
  # fetch by id for the full shape. Branch match via conditions.ref_name.include;
  # non_fast_forward is the force-push rule (no separate force_push rule exists).
  local ids id main_target types
  ids="$(gh api "repos/$PROTECTED_REPO/rulesets" | jq -c '[.[] | select(.target=="branch" and .enforcement=="active") | .id]')" \
    || fail "ruleset list API failed"
  [[ "$(jq 'length' <<<"$ids")" == "1" ]] \
    || fail "expected exactly 1 active branch ruleset, got $(jq 'length' <<<"$ids")"
  id="$(jq -r '.[0]' <<<"$ids")"
  local detail
  detail="$(gh api "repos/$PROTECTED_REPO/rulesets/$id")" \
    || fail "ruleset detail API failed (id=$id)"
  main_target="$(jq -r '(.conditions.ref_name.include // []) | index("refs/heads/main")' <<<"$detail")" \
    || fail "jq parse failed"
  [[ "$main_target" != "null" ]] || fail "ruleset $id does not target refs/heads/main"
  types="$(jq -r '[.rules[].type] | sort | join(",")' <<<"$detail")" \
    || fail "jq parse failed"
  [[ "$types" == *"deletion"* && "$types" == *"non_fast_forward"* ]] \
    || fail "ruleset rules incomplete: $types (need deletion + non_fast_forward)"
  echo "protect OK"
}

cmd_branch() {
  git -C "$REPO_DIR" fetch origin "$WORK_BRANCH" --quiet \
    || fail "fetch of $WORK_BRANCH failed (not pushed?)"
  git -C "$REPO_DIR" rev-parse --verify "origin/$WORK_BRANCH" >/dev/null 2>&1 \
    || fail "origin/$WORK_BRANCH missing"
  local counts
  counts="$(git -C "$REPO_DIR" rev-list --left-right --count origin/main...jamro/main)" \
    || fail "rev-list failed"
  [[ "$counts" == "$(printf '0\t0')" ]] || fail "origin/main no longer exact mirror: $counts"
  # fork-state doc committed on the branch
  git -C "$REPO_DIR" show "origin/$WORK_BRANCH:docs/fork-state.md" \
    | grep -qF 'exact mirror' || fail "docs/fork-state.md not committed on $WORK_BRANCH"
  # .gitignore keeps the local (PII) research dir out of the public fork
  git -C "$REPO_DIR" show "origin/$WORK_BRANCH:.gitignore" \
    | grep -qF '/docs/research/' || fail ".gitignore does not exclude /docs/research/"
  # privacy guard: the PII-laden OSINT report must NOT be committed anywhere
  if git -C "$REPO_DIR" show "origin/$WORK_BRANCH:docs/research/tiny-engineer-osint-2026-08-29.md" \
      >/dev/null 2>&1; then
    fail "PII report committed on $WORK_BRANCH (privacy violation)"
  fi
  echo "branch OK"
}

cmd_verdict() {
  [[ -f "$OPS_FILE" ]] || fail "missing $OPS_FILE"
  grep -qF 'long-standing Krakow maker' "$OPS_FILE" || fail "final verdict text missing in daily-ops file"
  grep -qF 'fork remains an exact mirror' "$OPS_FILE" || fail "verdict satware angle missing"
  grep -qiF 'ruleset' "$OPS_FILE" || fail "branch-protection action not recorded"
  grep -qF "$WORK_BRANCH" "$OPS_FILE" || fail "working branch not recorded"
  echo "verdict OK"
}

case "${1:-}" in
  drift) cmd_drift ;;
  forks) cmd_forks ;;
  selftest) cmd_selftest ;;
  report) cmd_report ;;
  sod) cmd_sod ;;
  issue) cmd_issue ;;
  protect) cmd_protect ;;
  branch) cmd_branch ;;
  verdict) cmd_verdict ;;
  osint-report|confidence|identity|deep|ops-ref)
    case "${1:-}" in
      osint-report) cmd_osint_report ;;
      confidence) cmd_confidence ;;
      identity) cmd_identity ;;
      deep) cmd_deep ;;
      ops-ref) cmd_ops_ref ;;
    esac ;;
  -h|--help|help) show_help ;;
  *) show_help; fail "unknown subcommand: ${1:-<none>}" ;;
esac
