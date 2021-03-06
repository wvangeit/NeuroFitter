/******************************************************************************
* NOMAD -  Nonlinear Optimization for Mixed vAriables and Derivatives.        *
* Copyright (C) 2001-2004  Gilles Couture - Ecole Polytechnique,              *
*                          Charles Audet - Ecole Polytechnique,               *
*		           John Dennis - Rice University.                     *
*                                                                             *
* Author: Gilles Couture                                                      *
* �cole Polytechnique de Montr�al - GERAD                                     *
* C.P. 6079, Succ. Centre-ville, Montr�al (Qu�bec) H3C 3A7 Canada             *
* Tel:  (514) 340-6053 x6928                                                  *
* Email: Gilles.Couture@gerad.ca                                              *
* Fax: (514) 340-5665                                                         *
*                                                                             *
* This program is free software; you can redistribute it and/or               *
* modify it under the terms of the GNU General Public License                 *
* as published by the Free Software Foundation. You can find the              *
* GNU General Public License at:                                              *
*             http://www.gnu.org/copyleft/gpl.html                            *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with this program; if not, write to the Free Software                 *
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. *
*                                                                             *
* You can find information on the NOMAD software at:                          *
*             http://www.gerad.ca/NOMAD/                                      *
******************************************************************************/

// cachecontrol.cpp -- CacheControl class methods.

#include "berkeleydb.h"
#include "cachecontrol.h"
#include "database.h"
#include "defines.h"

/* Constructor. Two caches (BerkeleyDB objects) are created.
   The CacheControl object receives these parameters:
   - 'dim' is the problem dimension (number of variables).
   - 'out' is the number of outputs of the problem.
   - 'perm' is the path of the permanent cache. */
CacheControl::CacheControl(const unsigned int dim, const unsigned int out,
			   const char * perm)
{
  display_factor = 0;
  // The caches are created here.
  perm_dbase = new BerkeleyDB(perm, dim, out);
  temp_dbase = new BerkeleyDB("LIBRARY/tempcache.dbs", dim, out);
}

// Destructor.
CacheControl::~CacheControl()
{
  // We destroy the databases if they exist.
  if (perm_dbase != NULL)
    delete perm_dbase;
  if (temp_dbase != NULL)
    {
      delete temp_dbase;
      system("rm -f LIBRARY/tempcache.dbs");
    }
}

// This method inserts a point in the caches.
bool CacheControl::insert(double * inp, double * utp)
{
  bool temp, perm;
  temp = temp_dbase->insert(inp, utp);
  perm = perm_dbase->insert(inp, utp);
  if ((temp == 0) && (perm == 0) )
    return true;
  else
    return false;
}

// This method checks if the databases contain a point.
unsigned int CacheControl::contains(double * inp, double * utp)
{
  if (temp_dbase->contains(inp, utp) == 0)
    {
      if (display_factor >= 5)
	cout << "Temporary cache hit!\n";
      return TEMP_CACHE_HIT;
    }
  else
    if (perm_dbase->contains(inp, utp) == 0)
      {
	if (display_factor >= 5)
	  cout << "Permanent cache hit!\n";
	return PERM_CACHE_HIT;
      }
    else
      return NO_CACHE_HIT;
}

// Returns the number of hits in the permanent cache.
unsigned int CacheControl::getPermCacheHits()
{
  return perm_dbase->getHits();
}

// Returns the number of hits in the temporary cache.
unsigned int CacheControl::getTempCacheHits()
{
  return temp_dbase->getHits();
}

// The caches are closed.
void CacheControl::closeCaches()
{
  // The content of the buffer is written in the permanent cache.
  perm_dbase->sync();
  // The temporary cache is destroyed (the run is over).
  temp_dbase->truncate();
}

// The user wants to continue the current run.
void CacheControl::continueRun()
{
  perm_dbase->continueRun();
  temp_dbase->continueRun();
}

// The display factor is reset.
void CacheControl::setDisplay(const unsigned int display)
{
  display_factor = display;
}

// show -- Outputs the CacheManager object.
void CacheControl::showOutput(ofstream & fout)
{
  if (!fout.is_open() )
    return;
  fout << "Number of temporary cache hits: ";
  temp_dbase->showOutput(fout);
  fout << "Number of permanent cache hits: ";
  perm_dbase->showOutput(fout);
}
