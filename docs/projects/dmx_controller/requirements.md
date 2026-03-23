# DMX Controller Project Requirements

# Overview

FR-DMX-100: The system shall control a DMX universe from a foot switch with display feedback and browser-based configuration.

# Presets

FR-DMX-200: The system shall support at least 100 presets, each with a name and 512 DMX values (one per channel) and an enabled flag.

FR-DMX-210: The user can easily select one of the enabled presets or use the predefined Blackout preset (all channels 0) via the website. The website will show the current active preset.

FR-DMX-220: The user can change the enabled flag of a preset via the website.

FR-DMX-230: The user can change the name of a preset via the website.

FR-DMX-240: The user can move a preset up or down in the list to change the order via the website.

FR-DMX-250: The user can insert a new preset at any position in the list via the website.

FR-DMX-260: The user can delete a preset from the list via the website.

FR-DMX-270: The user can edit the DMX values of a preset via the website.

FR-DMX-280: The user can select the next preset by short pressing the foot switch, and long pressing the foot switch will select the previous enabled preset.

FR-DMX-290: The system shall support a circular navigation mode where the next preset after the last one is the first one, and the previous preset before the first one is the last one.

FR-DMX-300: The presets and current preset are stored in NVS to persist across reboots.

# Configuration

FR-DMX-400: The user can configure the system via the website.

FR-DMX-410: The user can configure the foot switch long press time.

FR-DMX-412: The user can configure the foot switch polarity.

FR-DMX-414: The user can configure the circular navigation toggle.

## Website

FR-DMX-500: The website can show the manual.

## Hardware

HW-DMX-900: The system shall be powered via a standard 5.5/2.1 (?) mm, center positive barrel plug connector.

HW-DMX-905: The power adapter shall supply a regulated voltage and sufficient current to operate all system components under full load
