# Molecular Dynamics Engine (C++)

A simple implementation of a molecular dynamics engine in C++ which simulates argon atoms using the Lennard-Jones potential and velocity-Verlet integration algorithms. 

## Features
- Implementation of velocity-Verlet algorithm
- Support for periodic boundary conditions
- Compute a variety of energy types
- Basic thermostate and temperature control
- Output '.xyz' trajectories and '.res' energy logs

## Build Instructions
Simply call the makefile

```bash
make
```

## Run Example
```bash
./simulation
```

## Output
- traj_path.xyz - Trajectory path which can be visualized in ChimeraX
- energy.res - Energy components per timestep

## Visualization
Here, we used ChimeraX to visualize trajectories

## Future works
- Add pressure equilibration and coupling
- Implementation of explicit bonded constraints
- Further experimental validation
- Implementation of Metal GPU to speed up computation
