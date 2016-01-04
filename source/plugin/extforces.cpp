/******************************************************************************
 *
 * MantaFlow fluid solver framework
 * Copyright 2011 Tobias Pfaff, Nils Thuerey 
 *
 * This program is free software, distributed under the terms of the
 * GNU General Public License (GPL) 
 * http://www.gnu.org/licenses
 *
 * Set boundary conditions, gravity
 *
 ******************************************************************************/

#include "vectorbase.h"
#include "grid.h"
#include "commonkernels.h"

using namespace std;

namespace Manta { 

// MLE 2014-07-05 copy from pressure.cpp
inline void convertDescToVec(const string& desc, Vector3D<bool>& lo, Vector3D<bool>& up) {
    for(size_t i=0; i<desc.size(); i++) {
        if (desc[i] == 'x') lo.x = true;
        else if (desc[i] == 'y') lo.y = true;
        else if (desc[i] == 'z') lo.z = true;
        else if (desc[i] == 'X') up.x = true;
        else if (desc[i] == 'Y') up.y = true;
        else if (desc[i] == 'Z') up.z = true;
        else errMsg("invalid character in boundary description string. Only [xyzXYZ] allowed.");
    }
}

//! add Forces between fl/fl and fl/em cells
KERNEL(bnd=1) void KnAddForceField(FlagGrid& flags, MACGrid& vel, Grid<Vec3>& force) {
	bool curFluid = flags.isFluid(i,j,k);
	bool curEmpty = flags.isEmpty(i,j,k);
	if (!curFluid && !curEmpty) return;
	
	if (flags.isFluid(i-1,j,k) || (curFluid && flags.isEmpty(i-1,j,k))) 
		vel(i,j,k).x += 0.5*(force(i-1,j,k).x + force(i,j,k).x);
	if (flags.isFluid(i,j-1,k) || (curFluid && flags.isEmpty(i,j-1,k))) 
		vel(i,j,k).y += 0.5*(force(i,j-1,k).y + force(i,j,k).y);
	if (vel.is3D() && (flags.isFluid(i,j,k-1) || (curFluid && flags.isEmpty(i,j,k-1))))
		vel(i,j,k).z += 0.5*(force(i,j,k-1).z + force(i,j,k).z);
}

//! add Forces between fl/fl and fl/em cells
KERNEL(bnd=1) void KnAddForce(FlagGrid& flags, MACGrid& vel, Vec3 force) {
	bool curFluid = flags.isFluid(i,j,k);
	bool curEmpty = flags.isEmpty(i,j,k);
	if (!curFluid && !curEmpty) return;
	
	if (flags.isFluid(i-1,j,k) || (curFluid && flags.isEmpty(i-1,j,k))) 
		vel(i,j,k).x += force.x;
	if (flags.isFluid(i,j-1,k) || (curFluid && flags.isEmpty(i,j-1,k))) 
		vel(i,j,k).y += force.y;
	if (vel.is3D() && (flags.isFluid(i,j,k-1) || (curFluid && flags.isEmpty(i,j,k-1))))
		vel(i,j,k).z += force.z;
}

//! add gravity forces to all fluid cells
PYTHON void addGravity(FlagGrid& flags, MACGrid& vel, Vec3 gravity) {    
	Vec3 f = gravity * flags.getParent()->getDt() / flags.getDx();
	KnAddForce(flags, vel, f);
}

//! add Buoyancy force based on smoke density
KERNEL(bnd=1) void KnAddBuoyancy(FlagGrid& flags, Grid<Real>& density, MACGrid& vel, Vec3 strength) {    
	if (!flags.isFluid(i,j,k)) return;
	if (flags.isFluid(i-1,j,k))
		vel(i,j,k).x += (0.5 * strength.x) * (density(i,j,k)+density(i-1,j,k));
	if (flags.isFluid(i,j-1,k))
		vel(i,j,k).y += (0.5 * strength.y) * (density(i,j,k)+density(i,j-1,k));
	if (vel.is3D() && flags.isFluid(i,j,k-1))
		vel(i,j,k).z += (0.5 * strength.z) * (density(i,j,k)+density(i,j,k-1));    
}

//! add Buoyancy force based on smoke density
PYTHON void addBuoyancy(FlagGrid& flags, Grid<Real>& density, MACGrid& vel, Vec3 gravity) {
	Vec3 f = - gravity * flags.getParent()->getDt() / flags.getParent()->getDx();
	KnAddBuoyancy(flags,density, vel, f);
}

//! add Buoyancy force based on smoke density
PYTHON void print_dt_dx(FlagGrid& flags, Grid<Real>& density, MACGrid& vel) {
//	Vec3 f = - gravity * flags.getParent()->getDt() / flags.getParent()->getDx();
	std::cout << " flags.getParent()->getDt() " << flags.getParent()->getDt() << std::endl ;
	std::cout << " flags.getParent()->getDx() " << flags.getParent()->getDx() << std::endl ;
}

//! set no-stick wall boundary condition between ob/fl and ob/ob cells
KERNEL void KnSetWallBcs(FlagGrid& flags, MACGrid& vel, Vector3D<bool> lo, Vector3D<bool> up, bool admm) {

	bool curFluid = flags.isFluid(i,j,k);
    bool curObstacle = flags.isObstacle(i,j,k);
	if (!curFluid && !curObstacle) return;

	// MLE 2014-07-04
	// if not admm, leave it as in orig
	// if openBound, don't correct anything (solid is as empty)
	// if admm, correct if vel is pointing outwards
	
	// if "inner" obstacle vel
	if(i>0 && curObstacle && !flags.isFluid(i-1,j,k)) vel(i,j,k).x = 0;
	if(j>0 && curObstacle && !flags.isFluid(i,j-1,k)) vel(i,j,k).y = 0;

	// check lo.x
	if(!lo.x && i>0 && curFluid && flags.isObstacle(i-1,j,k) && ((admm&&vel(i,j,k).x<0)||!admm)) vel(i,j,k).x = 0;
	// check up.x
	if(!up.x && i>0 && curObstacle && flags.isFluid(i-1,j,k) && ((admm&&vel(i,j,k).x>0)||!admm)) vel(i,j,k).x = 0;
	// check lo.y
	if(!lo.y && j>0 && curFluid && flags.isObstacle(i,j-1,k) && ((admm&&vel(i,j,k).y<0)||!admm)) vel(i,j,k).y = 0;
	// check up.y
	if(!up.y && j>0 && curObstacle && flags.isFluid(i,j-1,k) && ((admm&&vel(i,j,k).y>0)||!admm)) vel(i,j,k).y = 0;
	// check lo.z
	if(!lo.z && k>0 && curFluid && flags.isObstacle(i,j,k-1) && ((admm&&vel(i,j,k).z<0)||!admm)) vel(i,j,k).z = 0;
	// check up.z
	if(!up.z && k>0 && curObstacle && flags.isFluid(i,j,k-1) && ((admm&&vel(i,j,k).z>0)||!admm)) vel(i,j,k).z = 0;
	

	/* MLE consider later	
	if (curFluid) {
		if ((i>0 && flags.isStick(i-1,j,k)) || (i<flags.getSizeX()-1 && flags.isStick(i+1,j,k)))
			vel(i,j,k).y = vel(i,j,k).z = 0;
		if ((j>0 && flags.isStick(i,j-1,k)) || (j<flags.getSizeY()-1 && flags.isStick(i,j+1,k)))
			vel(i,j,k).x = vel(i,j,k).z = 0;
		if (vel.is3D() && ((k>0 && flags.isStick(i,j,k-1)) || (k<flags.getSizeZ()-1 && flags.isStick(i,j,k+1))))
			vel(i,j,k).x = vel(i,j,k).y = 0;
	}
	*/
}

// MLE 2014-07-04
//! set no-stick boundary condition on walls
PYTHON void setWallBcs(FlagGrid& flags, MACGrid& vel, string openBound="", bool admm=false) {
	Vector3D<bool> lo, up;
    convertDescToVec(openBound, lo, up);
    KnSetWallBcs(flags, vel, lo, up, admm);
} 
//! Kernel: gradient norm operator
KERNEL(bnd=1) void KnConfForce(Grid<Vec3>& force, const Grid<Real>& grid, const Grid<Vec3>& curl, Real str) {
	Vec3 grad = 0.5 * Vec3(        grid(i+1,j,k)-grid(i-1,j,k), 
								   grid(i,j+1,k)-grid(i,j-1,k), 0.);
	if(grid.is3D()) grad[2]= 0.5*( grid(i,j,k+1)-grid(i,j,k-1) );
	normalize(grad);
	force(i,j,k) = str * cross(grad, curl(i,j,k));
}

PYTHON void vorticityConfinement(MACGrid& vel, FlagGrid& flags, Real strength) {
	Grid<Vec3> velCenter(flags.getParent()), curl(flags.getParent()), force(flags.getParent());
	Grid<Real> norm(flags.getParent());
	
	GetCentered(velCenter, vel);
	CurlOp(velCenter, curl);
	GridNorm(norm, curl);
	KnConfForce(force, norm, curl, strength);
	KnAddForceField(flags, vel, force);
}

//! enforce a constant inflow/outflow at the grid boundaries
KERNEL void KnSetInflow(MACGrid& vel, int dim, int p0, const Vec3& val) {
	Vec3i p(i,j,k);
	if (p[dim] == p0 || p[dim] == p0+1)
		vel(i,j,k) = val;
}

//! enforce a constant inflow/outflow at the grid boundaries
PYTHON void setInflowBcs(MACGrid& vel, string dire, Vec3 value) {
	for(size_t i=0; i<dire.size(); i++) {
		if (dire[i] >= 'x' && dire[i] <= 'z') { 
			int dim = dire[i]-'x';
			KnSetInflow(vel,dim,0,value);
		} else if (dire[i] >= 'X' && dire[i] <= 'Z') {
			int dim = dire[i]-'X';
			KnSetInflow(vel,dim,vel.getSize()[dim]-1,value);
		} else 
			errMsg("invalid character in direction string. Only [xyzXYZ] allowed.");
	}
}

//! add Buoyancy force based on smoke density
KERNEL(bnd=1) void create_modifier_plane_horizontal(FlagGrid& flags, MACGrid& vel_modifier, Vec3 value) {    
	if (!flags.isFluid(i,j,k)) return;
	if (flags.isFluid(i,j,k))
	{
		vel_modifier(i,j,k).x = value.x;
		vel_modifier(i,j,k).y = value.y;
		vel_modifier(i,j,k).z = value.z;
	}
}


//! As the name suggests , create velocity modifier = plane horizontal velocity
PYTHON void create_velocity_modifier_plane_horizontal(FlagGrid& flags, MACGrid& vel_modifier, Vec3 value) {
	create_modifier_plane_horizontal(flags,vel_modifier,value);
}

//! add Buoyancy force based on smoke density
KERNEL(bnd=1) void modify_velocity(FlagGrid& flags, MACGrid& vel, Vec3 value) {    
	if (!flags.isFluid(i,j,k)) return;
	if (flags.isFluid(i,j,k))
	{
		vel(i,j,k).x = value.x;
		vel(i,j,k).y = value.y;
		vel(i,j,k).z = value.z;
	}
}

//! As the name suggests , add the value of velocity_modifier to the velocity
PYTHON void modify_velocity_plain_horizontal(FlagGrid& flags, MACGrid& vel, Vec3 value) {    
//	gridAdd<MACGrid,MACGrid>(vel , vel_modifier);
	modify_velocity(flags , vel , value);
}


} // namespace
