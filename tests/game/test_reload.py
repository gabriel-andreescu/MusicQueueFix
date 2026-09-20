import json
from pathlib import Path

import pytest
from bmk.testing import wait_for

pytestmark = pytest.mark.game

FIXTURES = Path(__file__).resolve().parents[2] / "assets/misc/reproduction"


def test_whiterun_reload(devbench, game_artifacts):
    """An earlier save must clear both of Whiterun's scripted music requests."""
    for name in ("Before_Battle", "Whiterun_Battle"):
        assert (FIXTURES / f"{name}.ess").is_file()

    def state():
        return devbench.call("inspect", {"kind": "music-queue-fix"})

    def snapshot(name):
        result = state()
        (game_artifacts / f"{name}.json").write_text(
            json.dumps(result, indent=2), encoding="utf-8"
        )
        return result

    devbench.load("Before_Battle", cell="QASmoke")
    clean = snapshot("clean")
    assert clean["scripted"] == clean["queued"] == 0 and not clean["current"], (
        "Restart Skyrim before running this test",
        clean,
    )

    devbench.load("Whiterun_Battle")
    wait_for(lambda: state()["current"], message="Civil War music did not start")
    battle = snapshot("battle")
    assert battle["scripted"] == battle["queued"] == 2, battle

    devbench.load("Whiterun_Battle")
    wait_for(lambda: state()["current"], message="Civil War music did not restart")
    battle_reloaded = snapshot("battle-reloaded")
    assert battle_reloaded["scripted"] == battle_reloaded["queued"] == 2, (
        battle_reloaded
    )

    # Allow the music thread to finish its pending fade and queue removal.
    devbench.load("Before_Battle", cell="QASmoke", settle_ms=15000)
    reloaded = snapshot("reloaded")
    assert reloaded["scripted"] == 0, reloaded
    assert reloaded["queued"] == 0 and not reloaded["current"], reloaded
