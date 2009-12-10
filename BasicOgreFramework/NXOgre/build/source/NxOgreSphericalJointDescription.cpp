/** File: NxOgreSphericalJointDescription.cpp
    Created on: 14-Apr-09
    Author: Robin Southern "betajaen"
    SVN: $Id$

    � Copyright, 2008-2009 by Robin Southern, http://www.nxogre.org

    This file is part of NxOgre.

    NxOgre is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NxOgre is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with NxOgre.  If not, see <http://www.gnu.org/licenses/>.
*/

                                                                                       

#include "NxOgreStable.h"
#include "NxOgreSphericalJointDescription.h"
#include "NxOgreJointDescription.h"

                                                                                       

namespace NxOgre_Namespace
{

                                                                                       

SphericalJointDescription::SphericalJointDescription(void)
{
 reset();
}

SphericalJointDescription::~SphericalJointDescription(void)
{
}

void SphericalJointDescription::reset(void)
{
 JointDescription::reset();
 mSwingAxis.set(0,0,1);
 mProjectionDistance = 1.0f;
 mSphericalJointFlags = 0;
 mProjectionMode = Enums::JointProjectionMode_None;;
}

bool SphericalJointDescription::isValid(void) const
{
 return true; // temp.
}

                                                                                       

} // namespace NxOgre_Namespace

                                                                                       
