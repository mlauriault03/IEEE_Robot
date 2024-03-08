# Files:

robot.ino
- Entry point for the Arduino
- Set hardware-related constants
- setup() -- does setup you should never have to change unless hardware changes
- setupXX() -- setup a specific feature; called in setup()
- loop() -- makes a single function call, then enters an infinite loop
- testXX() -- runs a specific, low-level test (drive, move servos, etc)
    * Tests should be automic
    * Together, tests should verify the functionality of everything the program
    expects the robot to do (complete coverage).
- writeDrive() - Writes HIGH or LOW to specific stepper motors
- writeServos() - Set the servos "forward" or "sideways"
- writeDisplay() - Print something to the display (add other display functions)

robot.h
- declare publicly used functions from robot.ino

behaviors.cpp
- Implements the algorigms for robot behavior

mock.cpp
- Overrides arduino-specific functions (i.e., robot.ino) for testing. Include only when debugging.

test.cpp
- runs unit tests (using a very simple test framework)

mechanics.cpp
- implements phisical maneuvers that are too complicated to leave untested
(hence they are isolated from robot.ino)
