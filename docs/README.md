# East Coast Community Ocean Forecast System (ECCOFS)

![ROMS_Picture](https://github.com/myroms/roms/assets/23062912/d72765ed-9d55-4109-84fc-c51b05832adb)

# License

**Copyright (c) 2002-2026 The ROMS Group**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Change Logs

[ROMS ECCOFS Change Logs](https://github.com/myroms/roms_eccofs/blob/develop/docs/ROMS_ChangeLog.md)

# Overview

The **ECCOFS** (East Coast Community Ocean Forecast System) is a collaboration between Rutgers University,
University of California at Santa Cruz, Fanthom Science, and the National Oceanography and Atmospheric
Administration (**NOAA**) to provide real-time ocean analysis and forecast for the U.S. East Coast. It is part of a 
broader effort to provide continuous, high-quality data to the [National Weather Service](https://www.weather.gov),
emergency managers, and commercial mariners. It is based on [ROMS](https://github.com/myroms/roms) and 
[ROMS-JEDI](https://github.com/myroms/roms) advanced 4-dimensional variatianal (4D-Var) data assimilation
frameworks.

Check **wiki** for more information:

```
https://github.com/myroms/roms_eccofs/wiki
https://github.com/myroms/roms/wiki
https://github.com/myroms/roms-jedi
```

# Instructions

The **ROMS** and **ROMS-JEDI** frameworks are intended for users interested in ocean modeling and prediction.
They require an extensive background in ocean dynamics, numerical modeling, advanced data assimilation,
and computing to configure systems, prepare required inputs, run, and analyze results to ensure proper
ocean forecast deliverable products. 

This **GitHub** repository's main objective is to provide the configuration and required data for a single
three-day 4D-Var data assimilation cycle for training, testing, scientific exchange, and benchmarking across 
various computer architectures, including cloud computing platforms such as AWS (Amazon Web Services), and running
in the [IOOS (Integrated Ocean Observing System)](https://ioos.noaa.gov) Cloud Sandbox.

---
Please, use the following command to download the **ROMS-ECCOFS** configuration:

``` make
git clone https://github.com/myroms/roms_eccofs.git                 (default)
git clone https://github.com/myroms/roms_eccofs.git <source_dir>
```
---
|     |     |     |     |     |    |
| --- | --- | --- | --- | --- | --- |
|<img height="50" alt="Rutgers-DMCS_logo" src="https://github.com/user-attachments/assets/96be3456-da77-46b8-8feb-a38f11bc3ac7" /> | <img height="125" alt="UCSC_logo" src="https://github.com/user-attachments/assets/70ab1525-959f-43d6-bd7c-c50942bfcf4c" /> | <img height="125" alt="Fathom_Science_logo" src="https://github.com/user-attachments/assets/34ce12ba-c386-47d8-8923-ba121258e8e5" /> | <img height="125" alt="NOAA_logo" src="https://github.com/user-attachments/assets/9c0cec64-7c95-4f27-ba61-6c9e2a09c3a0" /> | <img height="125" alt="ROMS_logo" src="https://github.com/user-attachments/assets/8e3c405d-7cc6-4087-9284-3cb17b4639af" /> | <img height="125" alt="ROMS-JEDI_logo" src="https://github.com/user-attachments/assets/88f43835-a46c-4077-afe0-aabb7e486689" />|
