"""
Audio utilities for recording and processing speech commands.
"""

import numpy as np
import sounddevice as sd
from scipy.io.wavfile import write
import tensorflow as tf


def play_beep(frequency=1000, duration=0.2, sample_rate=44100):
    """Play a beep sound to indicate recording start (REQUIRED)."""
    # TODO: Generate sine wave for beep
    # TODO: Play beep using sounddevice
    pass


def record_command(duration=1.0, sample_rate=16000):
    """Record audio command. Sample rate MUST be 16000Hz to match training data."""
    # TODO: Record audio using sounddevice
    # TODO: Return flattened audio array
    pass


def record_with_beep(duration=1.0, sample_rate=16000):
    """Play beep then record audio command."""
    # TODO: Call play_beep()
    # TODO: Call record_command()
    pass


def decode_audio(audio_binary):
    """Decode WAV audio file.
    (TensorSpec(
    shape=(None, 16000, None)
    , dtype=tf.float32, name=None),

    TensorSpec(shape=(None,)
    , dtype=tf.int32, name=None))
    """
    audio, _ = tf.audio.decode_wav(audio_binary)
    return tf.squeeze(audio, axis=-1)


def get_spectrogram(waveform):
    """Convert audio waveform to spectrogram using STFT."""
    # TODO: Pad waveform to consistent length (16000 samples)
    spect = tf.signal.stft(
        waveform, frame_length=255, frame_step=128
    )
    spect = tf.abs(spect)
    # TODO: Add channel dimension
    return spect[..., tf.newaxis]


def save_audio(audio, filename, sample_rate=16000):
    """Save audio to WAV file."""
    # TODO: Use scipy.io.wavfile.write()
    pass