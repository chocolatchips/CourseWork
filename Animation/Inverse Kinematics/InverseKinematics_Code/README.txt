Model classroom:
	- The classroom is set with the wall along the z = -0.2 plane. This is to create an offset to avoid z-fighting. The floor has the y-axis as the normal.
	- The blackboard is set against the wall along the z = -0.1 plane to avoid z-fighting with the wall and spline.

Spline:
	- The spline is created using the provided Hermite spline class.
	- The spline is projected along the z = 0.0 plane and contained within the blackboard.
		- To fit within the blackboard, the spline points must be within x = [-6.0, 6.0] and y = [-3.0, 3.0]. These bounds were set based on the provided spline examples.
		
Model character:
	- The character is modeled using hierarchical chains based of the root translation.
	- The legs are mirrored and the left arm is a mirror of the right without any rotations.
	- The end effector is modeled as a red sphere at the end of the right arm.
	
Inverse Kinematics Solver:
	- The solver uses the Jacobian pseudo-inverse approach by using (JJT)B = X* to solve for the new theta values.
		- The beta vector is found using LU decomposition.
	- The matrix and vector arithmetic is done using the Eigen library.
	
Character rest pose:
	- The character begins with all thetas set to 0.1 rad. This gives the character the appearance of the provided rest pose with a slightly curved right arm.
	
Interface commands:
	- The "system bob position <x y z>" command sets the character model's target point. This will overwrite any spline target points until system is reset.
	- The "simulator iksim read <spline.txt>" command will project a spline onto the blackboard.
		- When simulation is started, the character will move the right arm such that the end effector will move from rest position to trace the spline.