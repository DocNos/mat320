"""
Main application for voice-controlled turtle.
"""

import os
import sys
import numpy as np
import tensorflow as tf
from audio_utils import record_with_beep, get_spectrogram
from turtle_controller import TurtleController


class VoiceControlledTurtle:
    """Main application class."""

    def __init__(self, model_path='models/speech_commands_model'):
        """Initialize voice-controlled turtle application."""
        # TODO: Load the trained model
        # TODO: Load label names
        # TODO: Initialize turtle controller
        pass

    def predict_command(self, audio):
        """Predict command from audio."""
        # TODO: Convert audio to spectrogram
        # TODO: Get model prediction
        # TODO: Return predicted command string
        pass

    def run(self):
        """Run main control loop."""
        # TODO: Loop to record audio and control turtle
        # TODO: Use record_with_beep() to get audio
        # TODO: Predict command
        # TODO: Execute command on turtle
        pass


def main():
    """Main entry point."""
    # TODO: Check if model exists
    # TODO: Create and run application
    pass


if __name__ == '__main__':
    main()