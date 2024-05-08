#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const std::string& name) :
    BaseSystem(name), numParticles(0)
{
}


/// <summary>
/// Displays all particles and springs and box along y-axis
/// </summary>
/// <param name="mode">Display mode</param>
void ParticleSystem::display(GLenum mode)
{
    glPointSize(5.0f);
    // Points and lines turn black if lighting enabled when drawn
    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    
    for each(Particle part in particles) {
        glVertex3dv(part.getPosition());
    }
    glEnd();

    glBegin(GL_LINES);
    for each (Spring spring in springs) {
        Vector ps1;
        Vector ps2;
        getParticle(spring.getPOne()).getPosition(ps1);
        getParticle(spring.getPTwo()).getPosition(ps2);

        glVertex3d(ps1[0], ps1[1], ps1[2]);
        glVertex3d(ps2[0], ps2[1], ps2[2]);

    }
    glEnd();


    set_colour(0.25f, 0.25f, 0.25f);
    // Lighting turned on to colour ground
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glTranslated(10.0, -0.1, 0.0);
    glScaled(24.0, 0.1, 24.0);
    glutSolidCube(1.0);
    glPopMatrix();
}


/// <summary>
/// Function supports the following commads:
/// system <sys_name> dim <Number of Particles>
/// system <sys_name> particle <index> <mass> <x y z vx vy vz>
/// system <sys_name> all_velocities  <vx vy vz>
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int ParticleSystem::command(int argc, myCONST_SPEC char** argv)
{
    if (argc < 1) {
        animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
        return TCL_ERROR;
    }
    else if (strcmp(argv[0], "dim") == 0) {
        if (argc != 2) {
            animTcl::OutputMessage("Incorrect usage: dim <Number of Particles>");
            return TCL_ERROR;
        }
        numParticles = std::stoi(argv[1]);
        populateParticles(numParticles);
    }
    else if (strcmp(argv[0], "particle") == 0) {
        if (argc != 9) {
            animTcl::OutputMessage("Incorrect usage: particle <index> <mass> <x y z vx vy vz>");
            return TCL_ERROR;
        }
        addParticles(std::stoi(argv[1]), std::stod(argv[2]), std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]), std::stod(argv[6]), std::stod(argv[7]), std::stod(argv[8]));
    }
    else if (strcmp(argv[0], "all_velocities") == 0) {
        if (argc != 4) {
            animTcl::OutputMessage("Incorrect usage: all_velocities  <vx vy vz>");
            return TCL_ERROR;
        }
        setAllVelocities(std::stod(argv[1]), std::stod(argv[2]), std::stod(argv[3]));
    }
    return TCL_OK;
}

void ParticleSystem::getState(double* p) {}

void ParticleSystem::setState(double* p) {}


/// <summary>
/// Funciton clears all particles
/// </summary>
/// <param name="time">Time of reset</param>
void ParticleSystem::reset(double time)
{
    particles.clear();
    springs.clear();
}


/// <summary>
/// Function returns particle at given index
/// </summary>
/// <param name="index">Index of particle</param>
/// <returns>Particle at index</returns>
Particle ParticleSystem::getParticle(int index)
{
    return particles.at(index);
}


/// <summary>
/// Function returns all particles
/// </summary>
/// <returns>All particles</returns>
std::vector<Particle> ParticleSystem::getParticles()
{
    return particles;
}


/// <summary>
/// Function returns all springs
/// </summary>
/// <returns>All springs</returns>
std::vector<Spring> ParticleSystem::getSprings()
{
    return springs;
}


/// <summary>
/// Function returns spring at given index
/// </summary>
/// <param name="index">Index of spring</param>
/// <returns>Spring at index</returns>
Spring ParticleSystem::getSpring(int index)
{
    return springs.at(index);
}


/// <summary>
/// Fixes particle to current position
/// </summary>
/// <param name="index">Index of particle</param>
void ParticleSystem::fixParticle(int index)
{
    particles[index].toggleFix();
}


/// <summary>
/// Function updates particle velocity
/// </summary>
/// <param name="index">Index of particle</param>
/// <param name="v">New velocity</param>
void ParticleSystem::updatePartVelocity(int index, Vector v)
{
    if (!particles.at(index).isFixed()) {
        particles.at(index).setVelocity(v);
    }
}


/// <summary>
/// Function updates particle position
/// </summary>
/// <param name="index">Index of particle</param>
/// <param name="x">New position</param>
void ParticleSystem::updatePartPosition(int index, Vector x)
{
    if (!particles.at(index).isFixed()) {
        particles.at(index).setPosition(x);
    }
}


/// <summary>
/// Populates particle vector with particles at origin
/// </summary>
/// <param name="num">Number of particles</param>
void ParticleSystem::populateParticles(int num)
{
    for (int i = 0; i < num; i++) {
        addParticles(i, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }
}


/// <summary>
/// Function adds spring to current vector of springs
/// </summary>
/// <param name="spring">New spring</param>
void ParticleSystem::addSpring(Spring spring)
{
    springs.push_back(spring);
}


/// <summary>
/// If particle exists at index, overwrites particle values
/// else creates new particle at end of particles vector
/// </summary>
/// <param name="index">Index of particle</param>
/// <param name="mass">Mass of particle</param>
/// <param name="x">X position of particle</param>
/// <param name="y">Y position of particle</param>
/// <param name="z">Z position of particle</param>
/// <param name="vx">X velocity of particle</param>
/// <param name="vy">Y velocity of particle</param>
/// <param name="vz">Z velocity of particle</param>
void ParticleSystem::addParticles(int index, double mass, double x, double y, double z, double vx, double vy, double vz) {
    if (index >= particles.size())
        particles.push_back(Particle(x, y, z, vx, vy, vz, mass));
    else {
        particles.at(index).setMass(mass);
        particles.at(index).setPosition(x, y, z);
        particles.at(index).setVelocity(vx, vy, vz);
    }
       
}


/// <summary>
/// Function sets velocity of all particles
/// </summary>
/// <param name="vx">New X velocity</param>
/// <param name="vy">New Y velocity</param>
/// <param name="vz">New Z velocity</param>
void ParticleSystem::setAllVelocities(double vx, double vy, double vz) {
    for each (Particle part in particles) {
        part.setVelocity(vx, vy, vz);
    }
}