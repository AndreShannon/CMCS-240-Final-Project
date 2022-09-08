# Traffic Simulator

Authors: André Shannon, Maggie Dong, Ashley Frazier, and Jearice Black

Group project completed for cs 240 at University of Richmond


## cmsc240_s2020_final_foxtrot

###How to compile and run code:
To compile our code just run the make file in the terminal. To run the code, you
run the simulation with two arguments in the command line. The first one is the input
file, the second is the number for the initial seed.

###Design Decisions:
For the Random class, we decided to make its methods static, so that we can use its
methods in the other classes with having to create an instance of the Random class every
time.

For the Vehicle class, we decided for it to be a child of the VehicleBase class, so
that we can use the animator. There are two static methods in the class, setProbabilityofVehicleType and setProbabilityofTurningDirection. These method store the
information from the input file pertaining to vehicles. The reason we made them was so that we can store the numbers with having to create vehicle objects. We have to private methods that use the
random class and probability to figure out the type of the vehicle and the probability of a
vehicle turning right or left. We created an enum class for store the moving direction of
the car. We call this class Turning.  


For the Stoplight class, we decided there was going to be one static method in the class, setTimes.
This method functions the same way as the static methods in the vehicle class. So it just stores information (how long the lights will be green, yellow, or red) from the input file. We did this for the same reason as in the vehicle class, so we can store the values without having to create a stoplight object.

For the Roadway class, we decided to make this class control how the intersection
as well as the road operates. We are using the 2D array approach that Dr. Lawson
suggested. For left turns, we thought it would be more realistic if two vehicles coming from opposite directions were both turning left then both of them would turn and pass the other vehicle on the left (with the passenger doors closest to each other). With that in mind, the vehicles turning left will essentially go diagonal across the intersection, leaving room for a vehicle from the other direction to turn left. In this situation, we also felt like it would be appropriate for both the left turning vehicle to take the right away, so if one of them is longer than the other and takes longer to clear the intersection, then possible oncomming traffic would wait until both vehicles clear the intersection or whichever vehicle would be blocking the way. We also decided to store all the instances of vehicle in lists since adding and deleting elements from a list doesn't affect pointers to its elements as much as a vector would. Also when adding a new vehicle, we just store them in the list until there is space for them to move onto the roadway. We also are deleting vehicles after they move off the roadway so that the list doesn't get too long.

For the Simulation class, we decided to read from the input file for the values
of the variables. This class creates a Roadway object that simulates the traffic.
We use a for loop to keep updating the traffic and the time. You can step through the simulation with enter like in the testAnimator example.
