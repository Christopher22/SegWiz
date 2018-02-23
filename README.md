# SegWiz

SegWiz is a powerful annotation tool for segmentation tasks in machine learning context. Indifference to other approaches it is optimized to deal with freely chosen shapes and widely distributed image sources.

## Key features of this software:
- High performance on almost any operating system due to native implementation in C++ and Qt
- Many supported image formats: BMP, GIF, JPG, PNG, PBM, PGM, PPM, XBM, XPM
- Flexible configuration files allows smooth integration into working groups and support teamwork
- Highly customizable in terms of appearance, working style, and image content.

## Usage:
Call SegWiz with the path to the configuration file of the annotation task.

## An example configuration file:
```json
{
    "start": 51,
    "end": 100,
    "scaling": 4,
    "seed": 1595555286,
    "input": [
        {
            "exclude": [
            ],
            "include": [
                "*.png"
            ],
            "path": "/input_dir"
        }
    ],
    "labels": [
        {
            "color": {
                "b": 255,
                "g": 0,
                "r": 0
            },
            "name": "Annotation 1"
        },
        {
            "color": {
                "b": 0,
                "g": 255,
                "r": 0
            },
            "name": "Annotation 2"
        },
        {
            "color": {
                "b": 0,
                "g": 0,
                "r": 255
            },
            "name": "Annotation 3"
        }
    ],
    "output": {
        "path": "/output_dir",
        "pattern": "%1.png"
    }
}
```

## ToDo:
- A GUI editor for the configuration files
