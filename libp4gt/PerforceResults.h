/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PERFORCE_RESULTS
#define INC_PERFORCE_RESULTS

#include "PerforceAPI.h"
#include "UniStrDict.h"

#define RESULT_DICTIONARY		"dic"
#define RESULT_MESSAGE			"msg"
#define RESULT_ERROR			"err"

#define RESULT_KEY				"resultType"

/*
 * Class to store results of executing Perforce command.
 */
class PerforceResults : public VarArray {

public:

	/*
	 * Constructor.
	 */
						PerforceResults();

	/* 
	 * Destructor.
	 */
						~PerforceResults();

	/* 
	 * Return result as a single string.
	 */
	TCHAR				*GetText();

	/*
	 * Return true if results contain error(s)
	 */
	bool				HasErrors() {return hasErrors;};

	/*
	 * Append dictionary result.
	 */
	void				Append( StrDict* );

	/* 
	 * Append information message result.
	 */
	void				Append( char level, const char *msg );

	/*
	 * Append error message result.
	 */
	void				AppendError( const char *msg );

	/*
	 * Clear any old results
	 */
	void				Reset();


private:

	TCHAR				*text;
	bool				hasErrors;
};

#endif
