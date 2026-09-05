# Fork state

This repository is a satware AG fork of [jamro/tiny-engineer](https://github.com/jamro/tiny-engineer).

| Property | Value |
|----------|-------|
| Role | Fork development line: upstream `main` + satware work (multi-arch port, native tests, CI) |
| Upstream position | Never behind `jamro/main` (drift gate); ahead commits are fork work |
| Protection | Ruleset `protect-main` on `main` (no deletion, no force push) |
| Sync trigger | New upstream push - re-audit the new commits, then merge into `main` (no ff once diverged) |

## Verdict (2026-08-29, updated 2026-09-05)

Single-author hobby project, clean MIT license, no commercial strings.
The 2026-08-29 exact-mirror state ended when PR #1 squash-merged to
`main` (`071cef1`, 2026-08-30): `main` now carries the fork's own work
and the standing invariant is "not behind upstream" - ahead commits are
the fork's development, not drift.

2026-08-30: upstream pushed 4 commits mid-day (dead/wakeup/sleep
animations, BOM speaker link, hardware-for-software-engineers guide;
+2404/-150). Re-audit: all animation/docs work, no license or policy
change. Merged into `main` at `56037f3`, no conflicts (single
auto-merged file: `src/hardware/rgb.cpp`).

2026-09-05: upstream pushed 1 commit (`aaaa420`, desk pad screw hole
fix). Re-audit: mechanical CAD fix plus a licensing reorg - software
stays MIT (scope clarified in `LICENSING.md`); `3d_models/cad/` and
`3d_models/parts/` are now CERN-OHL-S-2.0 (commercial use allowed,
reciprocal on modified distributed products); the Tiny Engineer
name/logo are trademark-reserved and not licensed (`TRADEMARK.md`,
`REUSE.toml`). No restriction on the fork's development line: all
ahead commits are firmware/CI/docs (MIT scope), and factual references
(fork repo name, README attribution) stay within the permitted uses.
Merged into `main` at `11c9577`, no conflicts. Finding: upstream
committed `3d_models/.DS_Store` (macOS junk).

The full provenance audit of the project and its creator (2026-08-29) is
kept internal; only this non-personal fork state is tracked here.
