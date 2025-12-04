# MAT 320 Project 8: Voice-Controlled Turtle

Automated Speech Recognition Neural Network that controls turtle graphics with voice commands.

## Project Structure

```
proj8_EC/
├── data/                    # Dataset (auto-downloaded)
├── models/                  # Saved models
├── src/
│   ├── audio_utils.py      # Audio recording and processing
│   ├── train_model.py      # Model training script (skeleton)
│   ├── turtle_controller.py # Turtle control (skeleton)
│   └── main.py             # Main application (skeleton)
├── requirements.txt
└── README.md
```

## Setup

### 1. Install Dependencies

```bash
pip install -r requirements.txt
```

Required packages:
- tensorflow (>=2.10.0) - **NOT INSTALLED**
- numpy - **INSTALLED**
- scipy - **INSTALLED**
- sounddevice - **NOT INSTALLED**
- matplotlib - **INSTALLED**

### 2. Train the Model

```bash
cd src
python train_model.py
```

This will:
- Download Speech Commands dataset
- Train CNN on voice commands
- Save model to `models/speech_commands_model`

### 3. Run Voice-Controlled Turtle

```bash
cd src
python main.py
```

## Requirements

### Required Features
- [ ] Train speech recognition neural network
- [ ] Save and load model
- [ ] Turtle controlled by voice commands
- [ ] Beep sound before recording (IMPLEMENTED in audio_utils.py)
- [ ] "yes" command returns turtle to start (TODO in turtle_controller.py)

### Optional Extra Credit
- [ ] "jump" command: repeats previous command twice
- [ ] "back" command: does opposite of previous command

## Voice Commands

- **up** - Move turtle up
- **down** - Move turtle down
- **left** - Move turtle left
- **right** - Move turtle right
- **go** - Move forward
- **stop** - Stop (no movement)
- **yes** - Return to starting position
- **no** - Do nothing

## Resources

- [TensorFlow Audio Tutorial](https://www.tensorflow.org/tutorials/audio/simple_audio)
- [YouTube Tutorial](https://www.youtube.com/watch?v=m-JzldXm9bQ)
- [Speech Commands Dataset](https://ai.googleblog.com/2017/08/launching-speech-commands-dataset.html)

## Next Steps

1. Install missing packages (tensorflow, sounddevice)
2. Complete train_model.py implementation
3. Complete turtle_controller.py implementation
4. Complete main.py implementation
5. Train the model
6. Test voice control