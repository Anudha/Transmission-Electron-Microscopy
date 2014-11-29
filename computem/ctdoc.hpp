/*   ctdoc.cpp

------------------------------------------------------------------------
Copyright 2013, 2014 Earl J. Kirkland


This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

---------------------- NO WARRANTY ------------------
THIS PROGRAM IS PROVIDED AS-IS WITH ABSOLUTELY NO WARRANTY
OR GUARANTEE OF ANY KIND, EITHER EXPRESSED OR IMPLIED,
INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL THE AUTHOR BE LIABLE
FOR DAMAGES RESULTING FROM THE USE OR INABILITY TO USE THIS
PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA
BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR
THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH
ANY OTHER PROGRAM). 

-----------------------------------------------------------------------------

computem document class

started 31-dec-2012 ejk
move aber and samp dialog into here 4-may-2013 ejk
add expandCell() 12-may-2013 ejk
start adding autostem 1-sep-2013, working 23-sep-2013 ejk
fix output of autostem pix (different nx,ny size) 24-sep-2013 ejk
add OnNewDocument() with built in specimens (Si110, SrTiO, graphene)
    9-nov-2013 ejk
add save pix as text 16-nov-2013 ejk
add model3D mode 28-nov-2013 ejk
add load/save param menu+subroutine 8-feb-2014 ejk
add separate sparam[] with copy of last used param[] for output
    in case param[] gets editted after calcluation 11-feb-2014 ejk
add OnEditCoord() 18 to 27-may-2014  ejk
add DoSaveCoord() 29-may-2014 ejk
add 2D abb. phase error calc. 31-aug-2014 ejk
add OnViewxx() to scale image 11 to 18-sep-2014 ejk
last modified 18-sep-2014 ejk
*/

#ifndef CTDOC_HPP_INCLUDED	// only include this file if its not already
#define CTDOC_HPP_INCLUDED	// remember that this has been included

#include "computem.hpp"  //  header for this file
#include "ctview.hpp"    //  my view class

#include "floatTIFF.hpp"  // image file handler

enum{
        ID_SETUP_INCOSTEM = 2001,
        ID_SETUP_AUTOSTEM = 2002,
        ID_SETUP_AUTOCTEM = 2003,
        ID_SETUP_EXITWAVE = 2004,
        ID_SETUP_GENERAL  = 2005,
        ID_SETUP_CNM      = 2006,
        ID_SETUP_SAMPLE   = 2007,
        ID_CALCULATE	  = 2008,
        ID_SETUP_MODEL3D  = 2009,
        ID_SETUP_ABBPHASE = 2010,
        ID_EDIT_COORD	  = 2011,
        ID_SETUP_CBED     = 2012,
		ID_GREY_FULL	= 2013,
		ID_GREY_ONE		= 2014,
		ID_GREY_SET		= 2015,
		ID_GREY_LOG		= 2016
};

enum{ noDoc=-1, tiffDoc=1, xyzDoc=2 };

class ctDoc : public wxDocument
{
        DECLARE_DYNAMIC_CLASS( ctDoc )

public:
        ctDoc();
        ~ctDoc();

        floatTIFF myFile;
    
        int nx, ny;    // transmission function size (in pixels, may not be square)
        int nxb, nyb;  // bitmap image size after interpolation to square pixels
        int npix;      // number of pix; 1 for real and 2 for complex

        //-----  unit cell specimen parameters -------
        int *Znum, natom;
        float ax, by, cz;			// unit cell size
        float *x, *y, *z, *occ, *wobble;	//  atomic coordinates

        //------ super cell parameters
        int ncellx, ncelly, ncellz;
        int *Znums, natoms;
        float axs, bys, czs;			// unit cell size
        float *xs, *ys, *zs, *occs, *wobbles;	//  atomic coordinates

        //-------- 3D model parameters
        float sphere3d;

		double GreyLogConstant;   //  to scale log greyscale

        //--------- parameters ----------------------------------------
        //    param= working copy,   sparam= copy last used to save
        float *param, *sparam;   // to get image parameters, 
        int Nparams;             // to get maximum number of parameters

        //  STEM parameters ----------------------------------------
        int nxProbe, nyProbe;	// probe size in pixels
        int nxiSTEM, nyiSTEM, nThick, ndetect;
        int n1pixr, n2pixr, n3pixr;	//  save current size of pixr
        float ***pixr;
        float **pacbedPix;        //  to save position averaged CBED 
        double xiSTEM, xfSTEM, yiSTEM, yfSTEM;

        cfpix pix;  //  working complex floating point image

        wxBitmap *ctBMP;  // a device dependent bitmap that can be sent to screen
        wxImage ctIMG;   // wx device independent image (cannot be drawn to screen!!)

        wxString docMessage;   //  final status bar message

        void makeBitmap( cfpix&, int LogGreyScale=0 );
        float tinter( cfpix &px, double x, double y, int ir );
        unsigned char *pixData;   //  for making display image=bitmap

        //---- for stream IO ; not used here
        //DocumentOstream& SaveObject(DocumentOstream& ostream);  // output file with result
        //DocumentIstream& LoadObject(DocumentIstream& istream);  // input data file
        //---- for traditional file IO ; used here
        bool DoOpenDocument(const wxString &  file );
        bool DoSaveDocument(const wxString &  file );
        bool OnNewDocument( );

        void DoSavePixText( const wxString &file, int imag );
        void DoSaveParam( const wxString &file );
        void DoLoadParam( const wxString &file );

        void OnSetAber( wxCommandEvent& WXUNUSED(event) );
        void OnSetGeneral( wxCommandEvent& event );
        void OnSetSample( wxCommandEvent& WXUNUSED(event) );

        void OnEditCoord( wxCommandEvent& WXUNUSED(event) );
        void DoSaveCoord( const wxString &file );

        void RUNincostem( );   //  to run (simple STEM) incostem mode
        void RUNautoslic( int lpartl, int lwobble );   //  to run autoslic mode (CTEM)
        void RUNautostem( int lwobble, ctCanvas* canvas );   //  to run autostem mode (STEM)
        void RUNmodel3d(  );    //  to run model 3D
        void RUNabbPhase(  );   //  to run abb. phase image in 2D
        void RUNcbed( ctCanvas* canvas );       //  to run cbed

        void OnGreyFull( wxCommandEvent& WXUNUSED(event) );
        void OnGreyOne( wxCommandEvent& WXUNUSED(event) );
        void OnGreySetRange( wxCommandEvent& WXUNUSED(event) );
        void OnGreyLog( wxCommandEvent& WXUNUSED(event) );

private:

        DECLARE_EVENT_TABLE()

        wxString ws;   //  to format messages

        int docType, sparamSet;

        void expandCell( );   //  to run incostem mode

        unsigned long iseed;   // for random number generator

        string xyzTitle;	// for top line of xyz coord. file with description

        //---- for 3D model
        void model3( float x[], float y[], float z[], 
                float ax, float by, float cz, float s[], int icolor[], int npts,
                float d, float size0, double rotat, double tilt, 
                cfpix &pix, float *scalepix );
        void sphere( double x0, double y0, double size, int icolor, 
                cfpix &l2pix, int nxout, int nyout );

};

#endif
