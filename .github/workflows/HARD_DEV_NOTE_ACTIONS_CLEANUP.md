# Hard Dev Note: GitHub Actions workflow list cleanup
If a workflow file has already been removed from Git but still appears in the GitHub Actions sidebar, clean it up in GitHub Actions metadata with the steps below.

## 1) Remove workflow file(s) on default branch
The Actions sidebar is driven by the default branch.
Delete/rename the unwanted workflow file(s) in `.github/workflows` on `master` (or current default branch), then push.

## 2) List all workflows (including stale/disabled)
```bash
gh workflow list --all -R harrypm/DomesdayDuplicator
```
Find the stale workflow ID (example: `Linux AppImage Build`).

## 3) Disable the stale workflow entry
```bash
gh workflow disable <workflow_id> -R harrypm/DomesdayDuplicator
```

## 4) Delete stale runs tied to that workflow
List runs:
```bash
gh run list -R harrypm/DomesdayDuplicator --workflow <workflow_id> --limit 100
```
Delete each run:
```bash
gh run delete <run_id> -R harrypm/DomesdayDuplicator
```

## 5) Verify final workflow list
```bash
gh workflow list --all -R harrypm/DomesdayDuplicator
```
Expected result: only active workflows you intend to keep (for this repo: `Build & Release`, `Build Linux`, `Build macOS`, `Build Windows`).

## 6) Build & Release manual controls
`Build & Release` supports manual dispatch with:
- `create_release` (boolean)
- `version_number` (string tag, example `v1.0.0`)

When `create_release=true`, the workflow creates/pushes the tag with `github-actions[bot]` if missing, then publishes release assets.
