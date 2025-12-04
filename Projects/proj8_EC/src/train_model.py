"""
Train speech recognition model on Speech Commands dataset.
Follows: https://www.tensorflow.org/tutorials/audio/simple_audio
"""

import os
import pathlib
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from audio_utils import decode_audio, get_spectrogram


# Constants
DATASET_PATH = 'data/mini_speech_commands'
BATCH_SIZE = 64
EPOCHS = 10
SEED = 42


def download_dataset():
    """Download and extract Speech Commands dataset."""
    # TODO: Use tf.keras.utils.get_file() to download dataset
    # URL: http://storage.googleapis.com/download.tensorflow.org/data/mini_speech_commands.zip
    pass


def get_label(file_path):
    """Extract label (command name) from file path."""
    # TODO: Split file path and return parent directory name
    pass


def get_waveform_and_label(file_path):
    """Load audio file and return waveform with label."""
    # TODO: Get label from file path
    # TODO: Read audio file
    # TODO: Decode audio
    pass


def get_spectrogram_and_label_id(audio, label, label_names):
    """Convert waveform to spectrogram and label to integer ID."""
    # TODO: Convert audio to spectrogram
    # TODO: Convert label string to integer index
    pass


def preprocess_dataset(files, label_names):
    """Create preprocessed dataset from file paths."""
    # TODO: Create tf.data.Dataset from file list
    # TODO: Map get_waveform_and_label
    # TODO: Map get_spectrogram_and_label_id
    pass


def build_model(input_shape, num_labels):
    """Build CNN model for speech recognition."""
    # TODO: Create Sequential model with:
    # - Resizing layer (32x32)
    # - Normalization layer
    # - Conv2D layers
    # - MaxPooling
    # - Dropout
    # - Dense layers
    pass


def plot_history(history):
    """Plot training history (accuracy and loss)."""
    # TODO: Create subplots for accuracy and loss
    # TODO: Plot train and validation metrics
    # TODO: Save figure
    pass


def main():
    """Main training function."""
    # TODO: Download dataset
    # TODO: Get command labels from folder names
    # TODO: Get all audio file paths
    # TODO: Split into train/val/test sets
    # TODO: Create preprocessed datasets
    # TODO: Build model
    # TODO: Compile model (Adam optimizer, SparseCategoricalCrossentropy loss)
    # TODO: Train model
    # TODO: Evaluate on test set
    # TODO: Plot training history
    # TODO: Save model to 'models/speech_commands_model'
    # TODO: Save label names to 'models/label_names.npy'
    pass


if __name__ == '__main__':
    main()