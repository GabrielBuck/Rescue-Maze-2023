# Rescue Maze 2023 — RoboCup Junior

**Autonomous rescue robotics · embedded control · sensor fusion · navigation**

Competition software developed by **Team Defenders** for **RoboCup Junior Rescue Maze**, culminating in the 2023 international event in Bordeaux, France.

I participated as **team captain and programmer**. The team earned a **Gold Medal in the Rescue Maze SuperTeam challenge** in Bordeaux. In the previous national cycle, Team Defenders also achieved **2nd place in Rescue Maze Primary at RoboCup Junior Brazil 2022**.

## Overview

Rescue Maze requires an autonomous robot to navigate an unknown environment, react to floor conditions and obstacles, identify rescue-related events, and make movement decisions under real-time constraints.

This repository contains the competition-era embedded software used to structure the robot's control loop around independent modules for sensing, locomotion, orientation, mapping and victim handling.

## Engineering scope

The codebase explores several robotics and embedded-systems problems:

- autonomous movement through an unknown maze;
- distance-based navigation and position correction;
- gyroscope-assisted turns and heading control;
- PID-based motion correction;
- encoder-based movement tracking;
- floor-color detection and behavioral responses;
- modular control of four Dynamixel motors;
- mapping and decision abstractions;
- victim-detection/rescue workflow integration.

## Software structure

```text
Main_Maze/
├── Main_Maze.ino     # Main control loop and decision flow
├── Motor.hpp         # Locomotion and Dynamixel motor control
├── Distancia.hpp     # Distance sensing and PID-related logic
├── Giroscopio.hpp    # Orientation and gyroscope handling
├── Sensores.hpp      # Sensor initialization and readings
├── Cor.hpp           # Floor-color detection
├── Mapeamento.hpp    # Mapping / movement decision abstraction
└── Vitimas.hpp       # Victim-related behavior
```

The main loop coordinates the robot as a state-driven system: receive a movement decision, perform turns or forward motion, continuously evaluate distance and floor conditions, correct orientation, and return new information to the mapping layer.

## Technical concepts

**Embedded programming:** Arduino/C++ · real-time control flow · hardware interfaces  
**Robotics:** autonomous navigation · sensor integration · motor control · encoders · gyroscope  
**Control:** PID-based correction · heading adjustment · state-driven behavior  
**Engineering process:** iterative testing · debugging under hardware constraints · competition-driven optimization

## Competition timeline

**RoboCup Junior Brazil 2022**  
Team: **Defenders**  
Category: **Rescue Maze Primary**  
Result: **2nd place**

**RoboCup Junior 2023 — Bordeaux, France**  
Team: **Defenders**  
Category: **Rescue Maze**  
Role: **Captain & Programmer**  
Result: **Gold Medal — Rescue Maze SuperTeam**

The 2022 national result is documented by Colégio Objetivo in its coverage of RoboCup Junior Brazil / CBR.

## Repository note

This repository preserves the software from the 2023 competition cycle as a **historical engineering artifact**. It reflects the hardware, libraries and iterative development constraints of that robot and is not maintained as a general-purpose robotics framework.

Its portfolio value is in the engineering decisions, modularization, control logic, leadership and problem-solving process developed for an international autonomous-robotics competition.
