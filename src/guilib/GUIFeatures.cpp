#include "guilib/GUIFeatures.h"
#include "gui/GUI.h"
#include "hmm/Observation.h"
#include "lib/io.h"
#include "features/RealFileFeatures.h"

CGUIFeatures::CGUIFeatures(CGUI * gui_)
  : gui(gui_), train_features(NULL), test_features(NULL), train_obs(NULL), test_obs(NULL)
{
}

CGUIFeatures::~CGUIFeatures()
{
    delete train_features;
    delete test_features;
    delete train_obs;
    delete test_obs;
}
		
bool CGUIFeatures::set_features(char* param)
{
    bool result=false;
    param=CIO::skip_spaces(param);
    char target[1024];
    char type[1024];
    int comp_features=1 ;

    if ((sscanf(param, "%s %s %i", type, target, &comp_features))>=2)
    {
	if ( (strcmp(target, "TRAIN")==0 && gui->guiobs.get_obs("POSTRAIN") && gui->guiobs.get_obs("NEGTRAIN")) ||
		(strcmp(target, "TEST")==0 && gui->guiobs.get_obs("POSTEST") && gui->guiobs.get_obs("NEGTEST")))
	{
	    CFeatures** f_ptr=NULL;
	    CObservation** o_ptr=NULL;
	    CObservation* pt=NULL;
	    CObservation* nt=NULL;

	    if (strcmp(target,"TRAIN")==0)
	    {
		f_ptr=&train_features;
		o_ptr=&train_obs;
		pt=gui->guiobs.get_obs("POSTRAIN") ;
		nt=gui->guiobs.get_obs("NEGTRAIN") ;
	    }
	    else if (strcmp(target,"TEST")==0)
	    {
		f_ptr=&test_features;
		o_ptr=&test_obs;
		pt=gui->guiobs.get_obs("POSTEST") ;
		nt=gui->guiobs.get_obs("NEGTEST") ;
	    }
	    else
		CIO::message("see help for parameters\n");

	    if (&f_ptr)
	    {
		if (strcmp(type,"TOP")==0)
		{
		    if (gui->guihmm.get_pos() && gui->guihmm.get_neg())
		    {

			CObservation* old_obs_pos=gui->guihmm.get_pos()->get_observations();
			CObservation* old_obs_neg=gui->guihmm.get_neg()->get_observations();

			delete (*o_ptr);
			*o_ptr=new CObservation(pt, nt);
			gui->guihmm.get_pos()->set_observations(*o_ptr);
			gui->guihmm.get_neg()->set_observations(*o_ptr);

			delete (*f_ptr);
			*f_ptr= new CTOPFeatures(gui->guihmm.get_pos(), gui->guihmm.get_neg());		      
			if (comp_features)
			    ((CTOPFeatures*)*f_ptr)->set_feature_matrix() ;

			//						gui->guihmm.get_pos()->set_observations(old_obs_pos);
			//						gui->guihmm.get_neg()->set_observations(old_obs_neg);

		    }
		    else
			CIO::message("HMMs not correctly assigned!\n");
		}
		else if (strcmp(type,"FK")==0)
		{
		    CIO::not_implemented();
		}
		else
		    CIO::not_implemented();
	    }
	}
	else
	    CIO::message("observations not correctly assigned!\n");
    }
    else
	CIO::message("see help for parameters\n");

    return result;
}

bool CGUIFeatures::load(char* param)
{
    param=CIO::skip_spaces(param);
    char filename[1024];
    char target[1024];
    char type[1024];
    bool result=false;

    if ((sscanf(param, "%s %s %s", filename, type, target))==3)
    {
	CFeatures** f_ptr=NULL;

	if (strcmp(target,"TRAIN")==0)
	{
	    f_ptr=&train_features;
	}
	else if (strcmp(target,"TEST")==0)
	{
	    f_ptr=&test_features;
	}
	else
	{
	    CIO::message("see help for parameters\n");
	    return false;
	}

	delete (*f_ptr);
	*f_ptr=NULL;

	if (strcmp(type,"REAL")==0)
	{
	    CIO::message("opening file...\n");
	    *f_ptr=new CRealFileFeatures(filename);
	}
	else
	{
	    CIO::message("unknown type\n");
	    return false;
	}

    } else
	CIO::message("see help for params\n");

    return result;
}

bool CGUIFeatures::save(char* param)
{
    bool result=false;
    param=CIO::skip_spaces(param);
    char filename[1024];
    char target[1024];

    if ((sscanf(param, "%s %s", filename, target))==2)
    {

	CFeatures** f_ptr=NULL;

	if (strcmp(target,"TRAIN")==0)
	{
	    f_ptr=&train_features;
	}
	else if (strcmp(target,"TEST")==0)
	{
	    f_ptr=&test_features;
	}
	else
	{
	    CIO::message("see help for parameters\n");
	    return false;
	}

	if (*f_ptr)
	{
	    FILE* file=fopen(filename, "w");
	    if ((!file) || (!(*f_ptr)->save(file)))
		CIO::message("writing to file %s failed!\n", filename);
	    else
	    {
		CIO::message("successfully written features into \"%s\" !\n", filename);
		result=true;
	    }

	    if (file)
		fclose(file);
	} else
	    CIO::message("set features first\n");

    } else
	CIO::message("see help for params\n");

    return result;
}
