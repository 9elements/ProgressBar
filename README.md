# Progress-Bar

[![Build](https://github.com/9elements/ProgressBar/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/9elements/ProgressBar/actions/workflows/build.yml)

## Overview

This repository contains the code for the firmware and the schematics for the ProgressBar automated payment system.
The device is a small IoT system based on Espressif ESP32-C3 equipped with an NFC scanner and components that ease user interaction like status LEDs.
While the code is in early development and not fixed to a stable release, expect breaking changes!

## Hardware

The device is based on Espressifs ESP32-C3, the schematics will be found in the `hardware` directory.

## Firmware

### Prerequisites

In order to build the firmware, you'll need to have the docker engine and dagger installed.
Docker can be installed on Linux via your distributions package manager or for other operating systems from [here](https://www.docker.com).
`dagger` installation instructions can be found [here](https://docs.dagger.io/install).
`dagger-cue` installation instructions can be found [here](https://docs.dagger.io/sdk/cue/).


### How to Build

First clone the repository:
```bash
git clone https://github.com/9elements/ProgressBar
```

Inside the folder prepare the build environment and then compile using:
```bash
dagger project update
dagger do build
```

## Backend

TODO

## Contributing

This project follows strict semantic versioning and requires [conventional commits](https://www.conventionalcommits.org/en/v1.0.0/)
