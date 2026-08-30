# 2026-08-30 EOD: gate-oracle ref freshness + gh issue-edit clobber

Two findings from the 2026-08-30 EOD, both caught and fixed the same day.

## Finding 1: orphaned comparison ref makes gates false-green (fixed)

`cmd_drift`/`cmd_branch` compared against `refs/remotes/jamro/main`, but
that ref is orphaned: no `jamro` remote is configured in the clone, and
the routine URL fetch (`git fetch <url> main`) updates `FETCH_HEAD`, not
that ref. The moment upstream moves, the gate keeps reading the stale ref
and reports "drift OK" - a false-green with no error signal.

**Caught live:** upstream jamro pushed 4 commits mid-day (+2404/-150:
dead/wakeup/sleep animations, BOM speaker link, engineer guide). The
pre-fix oracle read "0 behind" off the stale ref while the live upstream
head was 4 ahead of `main`. The fix runs against the live head, so the
same situation now reports "main is 4 behind" (true positive) and the
sync (merge `56037f3`) closed it.

**Fix:** `upstream_counts()` fetches the live upstream head per run and
compares `FETCH_HEAD`; fail-closed on fetch failure (an offline check
cannot prove freshness).

**Rule:** a comparison ref that no routine updates is a stale oracle.
Fetch-per-run against `FETCH_HEAD` (or query the live API) instead.

## Finding 2: `gh issue edit --body-file` clobbered an issue body (repaired)

During merge bookkeeping, `gh issue edit 2 --body-file <tmp>` replaced
issue #2's body with a freshly written template, dropping the existing
body content (SOD state, the "1 ahead" figure, marker references).

**Repaired:** issue #2 body + comments `5467725695` / `5467729362`
restored, and the stale "1 ahead" figure corrected to "3 ahead".

**Rule:** before `gh issue edit --body-file`, the file must be the
*existing* body (fetch via `gh api repos/<r>/issues/<n> --jq .body`) with
the edits applied - never a freshly written template.
