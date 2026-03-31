# System Requirements

## Hardware Architecture

FR-SYS-100: The system shall use a hardware architecture that is easily expandable to support many kinds of projects.

FR-SYS-110: New projects shall be able to reuse common hardware parts.

FR-SYS-120: The system shall support a common set of peripherals including GPIO, I2C, SPI, UART, and Wi-Fi for communication and control.

FR-SYS-130: The antenna power shall be sufficient to cover a typical venue or studio environment.

## Software Architecture

FR-SYS-200: New projects shall be able to reuse common software components.

## Website

FR-SYS-300: The website shall be reachable from devices on the same local network with or without a router.

FR-SYS-310: Wifi security shall be implemented to protect against unauthorized access while maintaining ease of use for the user. The user can change the WiFi password from the website.

FR-SYS-320: New projects shall be able to reuse the common website code and API contract for configuration, status monitoring, logging, and OTA updates.

FR-SYS-330: The website shall follow the [common API contract](common_api_contract.md), providing endpoints for configuration, status, logging, and OTA updates.

FR-SYS-350: The website shall show the title of the project, version, application specific buttons, and an application specific configuration button, and a generic Nodes, Security, Logging and application specific Manual button.

FR-SYS-360: Each non-home page shall show a Home button, and multi-level pages shall also show a Back button to return to the previous page.

FR-SYS-370: The system shall include a website to provide a user interface for configuration. The page has a Load and Save button.

FR-SYS-380: The website shall provide real-time status updates of the system. The following information will be shown: name, role, firmware version, MAC address, IP address, uptime, uptime, and watchdog status, last update time.

FR-SYS-380: The website shall support OTA updates for all nodes to allow for remote firmware updates without physical access with rollback support to prevent bricking devices. OTA only need to be supported when connected via a router/modem, not in direct AP mode. The OTA progress shall be shown on the website.

FR-SYS-390: The website shall display system logging events for debugging and monitoring purposes.

FR-SYS-400: The website shall provide a factory reset option.

FR-SYS-402: The website shall provide a restart of the system.

FR-SYS-410: In case of an error or failure of a button action, the website shall notify this.
