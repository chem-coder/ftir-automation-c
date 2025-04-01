# C Source Files

These C programs were written to control and interact with an FTIR spectrometer and Eurotherm temperature controller. Each file has a specific role in the automation and data acquisition process used during my PhD research.

- `AVRG_PID.c` – Sets PID parameters for the temperature controller
- `RAMP_ST.c` – Starts the programmed heating ramp
- `RD_PV.c` – Reads the current temperature from the controller
- `SAMDATA.c` – Processes and averages time/temperature data
- `W_SPID22.c` – Returns the system to ambient temperature settings
