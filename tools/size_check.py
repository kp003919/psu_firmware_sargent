name: PSU Firmware CI

on:
  push:
    branches: [ main, develop ]
  pull_request:

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout
      uses: actions/checkout@v4

    - name: Install ESP-IDF
      uses: espressif/esp-idf-ci-action@v1
      with:
        version: v5.2

    - name: Configure Target
      run: idf.py set-target esp32

    - name: Build Firmware
      run: idf.py build

    - name: Static Analysis (cppcheck)
      run: |
        sudo apt-get update
        sudo apt-get install -y cppcheck
        cppcheck --enable=all --error-exitcode=1 .
