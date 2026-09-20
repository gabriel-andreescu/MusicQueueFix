# Whiterun music reload reproduction

These saves reproduce battle music continuing after loading an earlier save.
Tested on Steam AE 1.7.104 with only the vanilla plugins.

## Setup

Disable Music Queue Fix and other music fixes or gameplay mods. Copy both `.ess`
files into your active save directory. If your mod manager uses profile-specific
saves, use that profile's directory. Otherwise, use
`Documents/My Games/Skyrim Special Edition/Saves`.

The saves require Skyrim's DLC, `_ResourcePack.esl`, and the free CC content
included with the AE version of the game: Fishing, Saints & Seducers, Rare
Curios and Survival Mode.

## Reproduce

1. Start the game with the provided saves installed.
2. From the main menu, open the console and enter `load Whiterun_Battle`. Let
   loading finish and wait for the battle music to play.
3. Enter `load Before_Battle` in the console.

You're now in QASmoke, but the battle music continues playing.

For comparison, exit Skyrim, start it again and load `Before_Battle` first.
There is no battle music.

To check the fix, enable Music Queue Fix, restart Skyrim and repeat steps 2–3.
The battle music should stop after loading `Before_Battle`.

## How these saves were made

The first save was made in QASmoke on a new character. The vanilla
`CWSiegeQuickStart` test quest, stage 4220, was used to set up the Stormcloak
assault on Whiterun. After teleporting to the battle, the character entered the
battle trigger and approached Galmar. The second save was made after his opening
speech finished, with both music requests active.
