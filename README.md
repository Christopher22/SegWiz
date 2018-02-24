# SegWiz

SegWiz is a powerful annotation tool for segmentation tasks in machine learning context. Indifference to other approaches it is optimized to deal with freely chosen shapes and widely distributed image sources.

## Key features of this software:
- High performance on almost any operating system due to its native implementation in C++ and Qt
- Many supported image formats: BMP, GIF, JPG, PNG, PBM, PGM, PPM, XBM, XPM
- Highly effective workflow due to shortcuts and usage of additional mouse buttons
- Flexible configuration files allow smooth integration into working groups and support teamwork
- Highly customizable in terms of appearance, working style, and image content.

## Usage:
Call SegWiz with the path to the configuration file of the annotation task.

## An example configuration file:
```json
{
    "start": 51,                    "(Specify the initial image of the annotation sequence. Default: 0)"
    "end": 100,                     "(Specify the last image of the annotation seqence. Default: Number of all available images)"
    "scaling": 4,                   "(Initial scaling applied on all loaded images. Default: 1)"
    "seed": 1595555286,             "(Seed for the random generator resulting in the same sequence across different devices. Default: Random number)"
    "input": [                      "(Input sources of images. Required)"
        {
            "exclude": [            "(Patterns for file which should be ignored)"
            ],
            "include": [            "(Patterns for file which should be included)"
                "*.png"
            ],
            "path": "/input_dir"    "(Directory containing the files)"
        }
    ],
    "labels": [                     "(Available labels. Required)"
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
    "output": {                     "(Configuration of output files)"
        "path": "/output_dir",      "(Directory containing the output maps in the end)"
        "pattern": "%1.png"         "(Naming pattern of the output files: %1 is replaced by the name of the input image without extension)"
    }
}
```

## ToDo:
- A GUI editor for the configuration files
