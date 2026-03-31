# GLOSSARY

| Term          | Definition                                   |
| ------------- | -------------------------------------------------------------------- |
| Application   | One application consisting of a master (node) and zero or more slave (node)s.  |
| Bridge        | The connection between the Master task and Slave tasks. |
| ESP-NOW       | The communication protocol used for communication between Master and Slave nodes. |
| ESP32         | The microcontroller used for both Master and Slave nodes. |
| ESP32-C3      | The microcontroller model that is typically used for both Master and Slave nodes. |
| Framework     | The common code (and hardware) that is independent of an application. |
| Functional Task | A task that performs a specific (hardware) function, such as GPIO control, MIDI, DMX, LED control, sensors etc. These tasks can run on either a Master or Slave node. |
| Master (Node) | The primary node in an application that manages the overall operation and coordination of the application. There can be only one Master Node in one application. |
| Master Task   | THe main task that runs only on the master node. |
| Node          | An ESP32, either a master or a slave node.  |
| Project       | The entire framework including all applications.          |
| Service Task  | A task that performs a generic service usable for all applications, such as OTA update, logging, status |
| Slave Task  | A task that runs on a slave node. There can be multiple Slave tasks in one application, but each slave node has its own slave task. |
| Task          | An RTOS task that can perform a function inside a Master or Slave node. |
| Slave (Node)  | A node in an application that is managed by the Master node and performs specific tasks or functions. There can be multiple Slave nodes in one application. |
| WebServer    | A web server that runs on the master node and serves a website to the user. |
| Website      | A website that is served by the WebServer and provides a user interface for the application. |
| WiFi         | A wireless networking technology that is used to communicate between the Master node and the website. |

# ABBREVIATIONS

| Abbreviation | Full Form                         |
| ------------ | --------------------------------- |
| **API**      | Application Programming Interface |
| **App**      | Application                       |
| **CLI**      | Command Line Interface            |
| **DMX**      | Digital Multiplex                 |
| **MIDI**     | Musical Instrument Digital Interface |
| **GUI**      | Graphical User Interface          |
| **HTTP**     | Hypertext Transfer Protocol       |
| **JSON**     | JavaScript Object Notation        |
| **OTA**      | Over The Air                      |
| **WiFi**     | Wireless Fidelity                 |
