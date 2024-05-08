Particle System:
	- ParticleSystem.h
		- Manages vectors of Particles and Springs
		- Draws Particles and Springs to canvas
		- Updates Particle and Spring values
	- ParticleSimulator.h
		- Runs the force calculations and integrations for the particles
		- Contains functions for Euler, Symplectic, and Verlet integration methods
		- Handles collision detection for Particles

Script Commands:
	- System
		- All system commands are implemented and funtional
	- Simulator
		- All simulator commands are implemented and funtional

Forward Euler:
	- Method is implemented in ParticleSimulator->eulerIntegrate
	- Each calculation of integration method is commented within function

Verlet:
	- Method is implemented in ParticleSimulator->verletIntegrate
	- Each calculation of integration method is commented within function

Symplectic Euler:
	- Method is implemented in ParticleSimulator->symplecticIntegrate
	- Each calculation of integration method is commented within function

Ground Forces:
	- Ground force calculations done in ParticleSimulator->calcNormalForce
	- Ground force calculation does not include friction
	- Ground force calculation includes check for ground collision
		- Uses (X dot N) < 0 for collision confirmation
		- N = <0, 1, 0>
	- Ground point for calculation is origin

Gravity:
	- Gravity force calculation handled in ParticleSimulator-calcGravity

Locking Particle's Position:
	- Particle's position lock can be toggled on/off
	- Force calculation and integration are skipped if particle is locked

Particle and Spring Drawing:
	- Drawing of both is performed in ParticleSystem-display
	- Particles are drawn using GL_POINTS of side 5.0f
	- Springs are drawn using GL_LINES between connected particle positions

Extras:
	- A rectangle is drawn along the y-axis to provide visual for ground