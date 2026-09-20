# In-game tests

Pytest checks Whiterun's two scripted music requests, repeated loads of the
battle save, and their removal when loading the earlier QASmoke save through
[DevBench](https://www.nexusmods.com/skyrimspecialedition/mods/181326).

## Requirements

- Python 3.11 or newer and [uv](https://docs.astral.sh/uv/).
- Steam AE 1.7.104 with its matching SKSE and Address Library, DevBench 1.5.0 or
  newer, and MusicQueueFix.
- The vanilla plugins listed in the
  [reproduction instructions](../../assets/misc/reproduction/README.md). Disable
  other music fixes and gameplay mods.
- Copy both `.ess` files from that reproduction folder into the active profile's
  save directory. Start a fresh game process for each run.

## Run

Install the locked Python dependencies:

```powershell
uv sync --locked
```

Start Skyrim through SKSE and leave it at the main menu, then run:

```powershell
uv run pytest tests/game --game-tests `
  --devbench-url http://127.0.0.1:8920 `
  --game-results build/game-results `
  --junitxml build/game-results.xml
```

Run serially, without pytest-xdist. Without `--game-tests`, the test is skipped
before connecting. Transcripts and music-state snapshots are written to
`--game-results`.

The test loads both saves and finishes in QASmoke.
