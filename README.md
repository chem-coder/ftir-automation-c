# FTIR Automation in C

This repository contains legacy C programs and macro scripts I developed to automate FTIR data collection as part of my PhD dissertation in Analytical Chemistry.

The code here was used to control a Eurotherm temperature controller and a Bruker FTIR spectrometer, enabling fully automated acquisition of temperature-programmed spectral data.

## 📂 Folder Structure

- `/src/` – C programs to control hardware and process data
- `/macros/` – Macro scripts used within the FTIR software environment

## 🔧 Programs Overview (C Code)

- `AVRG_PID.c` – Sets optimal PID parameters for heating ramp
- `RAMP_ST.c` – Starts the pre-programmed temperature ramp
- `RD_PV.c` – Reads current temperature from the Eurotherm TC
- `SAMDATA.c` – Averages time and temperature readings into `samdata.txt`
- `W_SPID22.c` – Resets the controller to ambient temperature conditions

## 🧠 Macros

- `SAMCOL_2.mac` – Full automation script for collecting VT-DRIFTS spectra; interfaces with C programs to manage temperature, scan timing, and data saving
- `NPROC2-1.mac` – Post-processing macro that references and processes spectra relative to a baseline

## 🧪 Scientific Context

This system was designed to support long-duration, temperature-controlled FTIR experiments. It allowed for unattended operation and synchronized logging of temperature/time data with each spectral scan.

These tools were crucial to the work presented in my dissertation on variable-temperature DRIFTS spectroscopy.

## ⚠️ Note

This code was developed for a specific lab setup and may require modification to work with other systems. Included here for archival and demonstration purposes.

