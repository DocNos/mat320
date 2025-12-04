"""
Turtle graphics controller for voice commands.
"""

import turtle


class TurtleController:
    """Controls turtle based on voice commands."""

    def __init__(self, step_size=50):
        """Initialize turtle controller."""
        # TODO: Initialize turtle screen and turtle object
        # TODO: Save starting position
        self.step_size = step_size
        self.previous_command = None

    def execute_command(self, command):
        """Execute a voice command."""
        # TODO: Implement movement for each command:
        # - up, down, left, right
        # - go, stop
        # - yes (return to start - REQUIRED)
        # - jump and back (OPTIONAL extra credit)
        pass

    def reset(self):
        """Reset turtle to starting position."""
        # TODO: Implement reset
        pass