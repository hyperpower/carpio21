#ifndef _GNUPLOT_HPP_
#define _GNUPLOT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>              // for std::ostringstream
#include <stdexcept>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>              // for getenv()
#include <list>                 // for std::list
#include <stdlib.h>
#include <type_define.hpp>
#include "io_define.hpp"
#include <memory>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
//defined for 32 and 64-bit environments
#include <io.h>                // for _access(), _mktemp()
#define GP_MAX_TMP_FILES  27   // 27 temporary files it's Microsoft restriction
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
//all UNIX-like OSs (Linux, *BSD, MacOSX, Solaris, ...)
#include <unistd.h>            // for access(), mkstemp()
#define GP_MAX_TMP_FILES  64
#else
#error unsupported or unknown operating system
#endif

namespace carpio {

//
class Gnuplot_actor {
protected:
	std::string _scmd;  //style comand
	std::string _pcmd;  //plot command
	std::list<std::string> _data;
public:
	friend class Gnuplot;
	/*
	 *  Constructor
	 */
	Gnuplot_actor() :
			_scmd(""), _pcmd(""), _data() {
	}
	Gnuplot_actor(const std::string& pcmd, const std::list<std::string>& data) :
			_scmd(""), _pcmd(pcmd), _data(data) {
	}
	Gnuplot_actor(const std::string& pcmd, const std::string& scmd,
			const std::list<std::string>& data) :
			_scmd(scmd), _pcmd(pcmd), _data(data) {
	}
	Gnuplot_actor(const Gnuplot_actor& ga) :
			_scmd(ga._scmd), _pcmd(ga._pcmd), _data(ga._data) {
	}
	/*
	 *  is empty
	 */
	bool empty() const {
		if (_pcmd == "") {
			return true;
		} else {
			return false;
		}
	}
	bool empty_style() const {
		if (_scmd == "") {
			return true;
		} else {
			return false;
		}
	}
	std::string& command() {
		return _pcmd;
	}
	const std::string& command() const {
		return _pcmd;
	}
	std::string& style() {
		return _scmd;
	}
	const std::string& style() const {
		return _scmd;
	}
	std::list<std::string>& data() {
		return _data;
	}
	const std::list<std::string>& data() const {
		return _data;
	}
	void clear() {
		_pcmd = "";
		_scmd = "";
		_data.clear();
	}
	void show_command() const {
		std::cout << "Actor command :" << _pcmd << "\n";
	}
	void show_data() const {
		for (std::list<std::string>::const_iterator ci = _data.begin();
				ci != _data.end(); ++ci) {
			std::cout << (*ci) << std::endl;
		}
	}

};

// A string tokenizer taken from http://www.sunsite.ualberta.ca/Documentation/
// /Gnu/libstdc++-2.90.8/html/21_strings/stringtok_std_h.txt
//
template<typename Container>
void stringtok(Container &container, std::string const &in,
		const char * const delimiters = " \t\n") {
	const std::string::size_type len = in.length();
	std::string::size_type i = 0;

	while (i < len) {
		// eat leading whitespace
		i = in.find_first_not_of(delimiters, i);

		if (i == std::string::npos)
			return;   // nothing left but white space

		// find the end of the token
		std::string::size_type j = in.find_first_of(delimiters, i);

		// push token
		if (j == std::string::npos) {
			container.push_back(in.substr(i));
			return;
		} else
			container.push_back(in.substr(i, j - i));
		// set up for next loop
		i = j + 1;
	}
	return;
}

class Gnuplot {
public:
	typedef std::shared_ptr<Gnuplot_actor> spActor;
	typedef std::list<std::shared_ptr<Gnuplot_actor> > list_spActor;
protected:
	/*
	 * \brief pointer to the stream that can be used to write to the pipe
	 */
	FILE *gnucmd;
	///\brief name of executed GNUPlot file
	std::string m_sGNUPlotFileName;
	///\brief gnuplot path
	std::string m_sGNUPlotPath;
	///\brief standart terminal, used by showonscreen
	std::string terminal_std;

	list_spActor _actors; // if plot() has external parameter, inner actors will be disabled

	bool _valid;
	/*
	 * Opens up a gnuplot session, ready to receive commands
	 */
	void _init() {
		//------------------------------------------------------------------------------
		//
		// initialize data
		//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		this->m_sGNUPlotFileName = "gnuplot.exe";
		this->m_sGNUPlotPath = "C:/Program Files/gnuplot/bin/";
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		this->m_sGNUPlotFileName = "gnuplot";
		this->m_sGNUPlotPath = "/usr/local/bin";
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		this->terminal_std = "windows";
#elif ( defined(unix) || defined(__unix) || defined(__unix__) ) && !defined(__APPLE__)
		this->terminal_std = "wxt";
#elif defined(__APPLE__)
		this->terminal_std = "qt";
#endif

		// char * getenv ( const char * name );  get value of environment variable
		// Retrieves a C string containing the value of the environment variable
		// whose name is specified as argument.  If the requested variable is not
		// part of the environment list, the function returns a NULL pointer.
#if ( defined(unix) || defined(__unix) || defined(__unix__) ) && !defined(__APPLE__)
		if (getenv("DISPLAY") == nullptr) {
			this->_valid = false;
			std::cerr << "!> Can't find DISPLAY variable! " << " \n";
		}
#endif

		// if gnuplot not available
		if (!this->_get_program_path()) {
			this->_valid = false;
			std::cerr << "!> Can't find gnuplot! " << " \n";
		}

		// FILE *popen(const char *command, const char *mode);
		// The popen() function shall execute the command specified by the string
		// command, create a pipe between the calling program and the executed
		// command, and return a pointer to a stream that can be used to either read
		// from or write to the pipe.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		std::string tmp = "\"" + this->m_sGNUPlotPath + "/" + this->m_sGNUPlotFileName +"\"" +
				+ " -persist";
		gnucmd = _popen(tmp.c_str(),"w");
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		std::string tmp = this->m_sGNUPlotPath + "/" + this->m_sGNUPlotFileName +
				+ " -persist";
		gnucmd = popen(tmp.c_str(), "w");
#endif

		// popen() shall return a pointer to an open stream that can be used to read
		// or write to the pipe.  Otherwise, it shall return a null pointer and may
		// set errno to indicate the error.
		if (!gnucmd) {
			this->_valid = false;
			std::cerr << "!> Couldn't open connection to gnuplot! " << " \n";
		}

		this->_valid = true;

		//set terminal type
		cmd("set output");
		cmd(
				"set terminal " + this->terminal_std
						+ " enhanced font 'Helvetica,12'");

		return;
	}
	/*
	 * Find out if a command lives in m_sGNUPlotPath or in PATH
	 */
	bool _get_program_path() {
		//
		// first look in m_sGNUPlotPath for Gnuplot
		//
		std::string tmp = this->m_sGNUPlotPath + "/" + this->m_sGNUPlotFileName;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		if ( this->_file_exists(tmp,0) ) // check existence
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		if (this->_file_exists(tmp, 1)) // check existence and execution permission
#endif
				{
			return true;
		}

		//
		// second look in PATH for Gnuplot
		//
		char *path;
		// Retrieves a C string containing the value of environment variable PATH
		path = getenv("PATH");

		if (path == NULL) {
			std::cerr << "!> PATH is not set! " << " \n";
			return false;
		} else {
			std::list<std::string> ls;
			//split path (one long string) into list ls of strings
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
			stringtok(ls, path,";");
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
			stringtok(ls, path, ":");
#endif
			// scan list for Gnuplot program files
			for (std::list<std::string>::const_iterator i = ls.begin();
					i != ls.end(); ++i) {
				tmp = (*i) + "/" + this->m_sGNUPlotFileName;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
				if ( this->_file_exists(tmp,0) ) // check existence
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
				if (this->_file_exists(tmp, 1)) // check existence and execution permission
#endif
						{
					this->m_sGNUPlotPath = *i; // set m_sGNUPlotPath
					return true;
				}
			}

			std::cerr << "Can't find gnuplot neither in PATH nor in \""
					<< this->m_sGNUPlotPath << " \n";

			this->m_sGNUPlotPath = " ";
			return false;
		}
	}

	/*
	 * Check if file exists
	 */
	bool _file_exists(const std::string &filename, int mode) {
		if (mode < 0 || mode > 7) {
			throw std::runtime_error(
					"In function \"Gnuplot::file_exists\": mode\
		                has to be an integer between 0 and 7");
			return false;
		}

		//  int _access(const char *path, int mode);
		//  returns 0 if the file has the given mode,
		//  it returns -1 if the named file does not exist or is not accessible in
		//  the given mode
		// mode = 0 (F_OK) (default): checks file for existence only
		// mode = 1 (X_OK): execution permission
		// mode = 2 (W_OK): write permission
		// mode = 4 (R_OK): read permission
		// mode = 6       : read and write permission
		// mode = 7       : read, write and execution permission
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		if (_access(filename.c_str(), mode) == 0)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		if (access(filename.c_str(), mode) == 0)
#endif
				{
			return true;
		} else {
			return false;
		}

	}
public:
	Gnuplot(){
		_init();
	}
	~Gnuplot() {
		// A stream opened by popen() should be closed by pclose()
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		if (_pclose(gnucmd) == -1) {
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		if (pclose(gnucmd) == -1) {
#endif
			std::cerr << " >! Problem closing communication to gnuplot \n";
		}
	}

	/*
	 * Sends a command to an active gnuplot session
	 */
	Gnuplot& cmd(const std::string &cmdstr) {
		if (!(this->_valid)) {
			return *this;
		}

		// int fputs ( const char * str, FILE * stream );
		// writes the string str to the stream.
		// The function begins copying from the address specified (str) until it
		// reaches the terminating null character ('\0'). This final
		// null-character is not copied to the stream.
		fputs((cmdstr + "\n").c_str(), gnucmd);

		// int fflush ( FILE * stream );
		// If the given stream was open for writing and the last i/o operation was
		// an output operation, any unwritten data in the output buffer is written
		// to the file.  If the argument is a null pointer, all open files are
		// flushed.  The stream remains open after this call.
		fflush(gnucmd);

		return *this;

	}

	/// turns grid on/off
	inline Gnuplot& set_grid() {
		cmd("set grid");
		return *this;
	}

	/// grid is not set by default
	inline Gnuplot& unset_grid() {
		cmd("unset grid");
		return *this;
	}
	inline Gnuplot& set_view(int rot_x, int rot_z, double scale,
			double scale_z) {
		std::ostringstream sst;
		sst << "set view " << rot_x << ", " << rot_z << ", " << scale << ", "
				<< scale_z;
		cmd(sst.str());
		return *this;
	}
	// only for splot
	inline Gnuplot& set_ticslevel(Vt v = 0.0) {
	        std::ostringstream sst;
	        sst << "set ticslevel " << v;
	        cmd(sst.str());
	        return *this;
	}
	// -----------------------------------------------
	/// set the mulitplot mode
	///
	/// \param ---
	///
	/// \return <-- reference to the gnuplot object
	// -----------------------------------------------
	inline Gnuplot& set_multiplot() {
		cmd("set multiplot");
		return *this;
	}

	// -----------------------------------------------
	/// unsets the mulitplot mode
	///
	/// \param ---
	///
	/// \return <-- reference to the gnuplot object
	// -----------------------------------------------
	inline Gnuplot& unset_multiplot() {
		cmd("unset multiplot");
		return *this;
	}

	// -----------------------------------------------------------------------
	/// \brief sets and clears the title of a gnuplot session
	///
	/// \param title --> the title of the plot [optional, default == ""]
	///
	/// \return <-- reference to the gnuplot object
	// -----------------------------------------------------------------------
	inline Gnuplot& set_title(const std::string &title = "") {
		std::string cmdstr;
		cmdstr = "set title \"";
		cmdstr += title;
		cmdstr += "\"";
		cmd(cmdstr);
		return *this;
	}

	//----------------------------------------------------------------------------------
	///\brief Clears the title of a gnuplot session
	/// The title is not set by default.
	///
	/// \param ---
	///
	/// \return <-- reference to the gnuplot object
	// ---------------------------------------------------------------------------------
	inline Gnuplot& unset_title() {
		this->set_title();
		return *this;
	}
	Gnuplot& set_xformat(const std::string &label = "%t") {
//      Tic-mark label numerical format specifiers
//		Format	    Explanation
//		%f	        floating point notation
//		%e or %E	exponential notation; an "e" or "E" before the power
//		%g or %G	the shorter of %e (or %E) and %f
//		%x or %X	hex
//		%o or %O	octal
//		%t	        mantissa to base 10
//		%l	        mantissa to base of current logscale
//		%s	        mantissa to base of current logscale; scientific power
//		%T	        power to base 10
//		%L	        power to base of current logscale
//		%S	        scientific power
//		%c	        character replacement for scientific power
//		%P	        multiple of pi
		std::ostringstream cmdstr;
		cmdstr << "set format x \"" << label << "\"";
		cmd(cmdstr.str());
		return *this;
	}

	Gnuplot& set_yformat(const std::string &label = "%t") {
		std::ostringstream cmdstr;
		cmdstr << "set format y \"" << label << "\"";
		cmd(cmdstr.str());
		return *this;
	}


	/// set x axis label
	Gnuplot& set_xlabel(const std::string &label = "x") {
		std::ostringstream cmdstr;

		cmdstr << "set xlabel \"" << label << "\"";
		cmd(cmdstr.str());

		return *this;
	}
	/// set y axis label
	Gnuplot& set_ylabel(const std::string &label = "y") {
		std::ostringstream cmdstr;

		cmdstr << "set ylabel \"" << label << "\"";
		cmd(cmdstr.str());

		return *this;
	}
	/// set z axis label
	Gnuplot& set_zlabel(const std::string &label = "z") {
		std::ostringstream cmdstr;

		cmdstr << "set zlabel \"" << label << "\"";
		cmd(cmdstr.str());

		return *this;
	}

	/// set z axis label
    Gnuplot& set_cblabel(const std::string &label = "c") {
        std::ostringstream cmdstr;

        cmdstr << "set cblabel \"" << label << "\"";
        cmd(cmdstr.str());

        return *this;
    }

	Gnuplot& set_equal_ratio() {
		std::ostringstream cmdstr;
		cmdstr << "set size ratio -1";
		cmd(cmdstr.str());

		return *this;
	}

	Gnuplot& set_label(
			int tag,
			const std::string & label,
			const double& x, const double& y,
			const std::string &append = "") {
		ASSERT(tag > 0);
		std::ostringstream cmdstr;
		cmdstr << "set label " << tag << " \"" << label << "\" at first " << x
				<< ",second " << y << " " << append;
		cmd(cmdstr.str());
		return *this;
	}
	template<class POINTS>
	Gnuplot& set_object2d(
	        int                tag,
	        const POINTS&      container,
	        const std::string& append = ""){
	    ASSERT(tag > 0);
		std::ostringstream cmdstr;
		cmdstr << "set object " << tag << " polygon from \\\n" ;
		for(auto iter = container.begin(); iter != container.end(); iter++){
		    auto p = *(iter);
		    auto iternext = std::next(iter, 1);
		    if (iternext != container.end()){
		        cmdstr << p[0] << "," << p[1] << " to \\\n";
		    }else{
		        cmdstr << p[0] << "," << p[1] << " \\\n";
		    }
		}
		cmdstr << append;
		cmd(cmdstr.str());
		return *this;
	}

	template<class POINTS>
    Gnuplot& set_object2dp( // container include pointer of points
            int tag,
            const POINTS& container,
            const std::string& append = "") {
        ASSERT(tag > 0);
        std::ostringstream cmdstr;
        cmdstr << "set object " << tag << " polygon from \\\n";
        for (auto& pp : container) {
            auto p = *(pp);
            cmdstr << p[0] << "," << p[1] << " to \\\n";
        }
        auto first = *(container.front());
        cmdstr << first[0] << "," << first[1] << "\\\n";
        cmdstr << append;
        cmd(cmdstr.str());
        return *this;
    }

	Gnuplot& set_equal_aspect_ratio(){
		cmd("set size ratio -1");
		return *this;
	}
	//------------------------------------------------------------------------------
	//
	Gnuplot& set_terminal_pdf(const std::string& filename, double x = 400,
			double y = 300, const std::string& font = "Helvetica",
			int fontsize = 12) {
		this->terminal_std = "pdfcairo";
		std::stringstream sst;
		sst << "set terminal " << this->terminal_std << " enhanced font '"
				<< font << "," << fontsize << "'" << "size " << x << ", " << y;
		cmd(sst.str());
		cmd("set output '" + filename + "'");
		return *this;
	}

	Gnuplot& set_terminal_png(
			const std::string& filename,
			           double  x         = 800,
			           double  y         = 600,
			const std::string& font      = "Fira Code",
			              int  fontsize  = 12) {
		this->terminal_std = "pngcairo";
		std::stringstream sst;
		sst << "set terminal " << this->terminal_std << " enhanced font '"
				<< font << "," << fontsize << "'" << "size " << x << ", " << y;
		cmd(sst.str());
		cmd("set output '" + filename + ".png'");
		return *this;
	}

	Gnuplot& set_terminal_jpeg(
			const std::string& filename,
			double x = 400, double y = 300,
			const std::string& font = "Helvetica",
			int fontsize = 12) {
		this->terminal_std = "jpeg";
		std::stringstream sst;
		sst << "set terminal " << this->terminal_std << " enhanced font '"
				<< font << "," << fontsize << "'" << "size " << x << ", " << y;
		cmd(sst.str());
		cmd("set output '" + filename + "'");
		return *this;
	}

	// set
	Gnuplot& set(const std::string& str) {
		std::ostringstream cmdstr;
		cmdstr << "set " << str;
		cmd(cmdstr.str());

		return *this;
	}

	Gnuplot& set_palette_blue_red() {
		std::ostringstream cmdstr;
		cmdstr
				<< "set palette defined(\
			0       0.2314  0.2980  0.7529,\
			0.03125 0.2667  0.3529  0.8000,\
			0.0625  0.3020  0.4078  0.8431,\
			0.09375 0.3412  0.4588  0.8824,\
			0.125   0.3843  0.5098  0.9176,\
			0.15625 0.4235  0.5569  0.9451,\
			0.1875  0.4667  0.6039  0.9686,\
			0.21875 0.5098  0.6471  0.9843,\
			0.25    0.5529  0.6902  0.9961,\
			0.28125 0.5961  0.7255  1.0000,\
			0.3125  0.6392  0.7608  1.0000,\
			0.34375 0.6824  0.7882  0.9922,\
			0.375   0.7216  0.8157  0.9765,\
			0.40625 0.7608  0.8353  0.9569,\
			0.4375  0.8000  0.8510  0.9333,\
			0.46875 0.8353  0.8588  0.9020,\
			0.5     0.8667  0.8667  0.8667,\
			0.53125 0.8980  0.8471  0.8196,\
			0.5625  0.9255  0.8275  0.7725,\
			0.59375 0.9451  0.8000  0.7255,\
			0.625   0.9608  0.7686  0.6784,\
			0.65625 0.9686  0.7333  0.6275,\
			0.6875  0.9686  0.6941  0.5804,\
			0.71875 0.9686  0.6510  0.5294,\
			0.75    0.9569  0.6039  0.4824,\
			0.78125 0.9451  0.5529  0.4353,\
			0.8125  0.9255  0.4980  0.3882,\
			0.84375 0.8980  0.4392  0.3451,\
			0.875   0.8706  0.3765  0.3020,\
			0.90625 0.8353  0.3137  0.2588,\
			0.9375  0.7961  0.2431  0.2196,\
			0.96875 0.7529  0.1569  0.1843,\
			1       0.7059  0.0157  0.1490\
			)";
		cmd(cmdstr.str());

		return *this;
	}


	Gnuplot& set_palette_blues() {
		std::ostringstream cmdstr;
		cmdstr<< "set palette defined(0 '#F7FBFF',\
    	    	      1 '#DEEBF7',\
		      	      2 '#C6DBEF',\
		              3 '#9ECAE1',\
		              4 '#6BAED6',\
		              5 '#4292C6',\
		              6 '#2171B5',\
		              7 '#084594' \
					  )";
		cmd(cmdstr.str());

		return *this;
	}

	Gnuplot& set_palette_red_grey() {
		std::ostringstream cmdstr;
		cmdstr << "set palette defined ( 0 '#B2182B',\
    	    	      1 '#D6604D',\
		      	      2 '#F4A582',\
		              3 '#FDDBC7',\
		              4 '#E0E0E0',\
		              5 '#BABABA',\
		              6 '#878787',\
		              7 '#4D4D4D' )";
		cmd(cmdstr.str());

		return *this;
	}
	// set range
	// set the xrange
	Gnuplot& set_xrange(const double iFrom, const double iTo) {
		std::ostringstream cmdstr;
		cmdstr << "set xrange[" << iFrom << ":" << iTo << "]";
		cmd(cmdstr.str());
		return *this;
	}

	Gnuplot& set_xrange_reverse(const double iFrom, const double iTo) {
		std::ostringstream cmdstr;
		cmdstr << "set xrange[" << iFrom << ":" << iTo << "] reverse";
		cmd(cmdstr.str());
		return *this;
	}

	Gnuplot& set_yrange(const double iFrom, const double iTo) {
		std::ostringstream cmdstr;
		cmdstr << "set yrange[" << iFrom << ":" << iTo << "]";
		cmd(cmdstr.str());
		return *this;
	}

	Gnuplot& set_yrange_reverse(const double iFrom, const double iTo) {
		std::ostringstream cmdstr;
		cmdstr << "set yrange[" << iFrom << ":" << iTo << "] reverse";
		cmd(cmdstr.str());
		return *this;
	}

	Gnuplot& set_zrange(const double iFrom, const double iTo) {
		std::ostringstream cmdstr;
		cmdstr << "set zrange[" << iFrom << ":" << iTo << "]";
		cmd(cmdstr.str());
		return *this;
	}

	Gnuplot& set_zrange_reverse(const double iFrom, const double iTo) {
		std::ostringstream cmdstr;
		cmdstr << "set zrange[" << iFrom << ":" << iTo << "] reverse";
		cmd(cmdstr.str());
		return *this;
	}

	Gnuplot& set_cbrange(const double iFrom, const double iTo) {
		std::ostringstream cmdstr;
		cmdstr << "set cbrange[" << iFrom << ":" << iTo << "]";
		cmd(cmdstr.str());

		return *this;
	}

	/// turns on/off log scaling for the specified xaxis (logscale is set by default)
	Gnuplot& set_xlogscale(const double base = 10){
		std::ostringstream cmdstr;

			cmdstr << "set logscale x " << base;
			cmd(cmdstr.str());

		return *this;
	}
	/// turns on/off log scaling for the specified yaxis (logscale is set by default)
	Gnuplot& set_ylogscale(const double base = 10) {
		std::ostringstream cmdstr;

		cmdstr << "set logscale y " << base;
		cmd(cmdstr.str());

		return *this;

	}
	/// turns on/off log scaling for the specified zaxis (logscale is set by default)
	Gnuplot& set_zlogscale(const double base = 10) {
		std::ostringstream cmdstr;

		cmdstr << "set logscale z " << base;
		cmd(cmdstr.str());

		return *this;

	}
	/*
	 *  plot
	 */
	template<typename CONTAINER>
	Gnuplot& plot_1(     //
			const CONTAINER& x,  //
			const std::string &str = "") {
		std::ostringstream ss;
		ss << "plot \"-\" using 1 " << str << "\n";
		cmd(ss.str());
		ss.str("");
		for (typename CONTAINER::const_iterator it = x.begin(); it != x.end();
				++it) {
			ss << (*it) << "\n";
			cmd(ss.str());
			ss.str("");
		}
		cmd("e\n");
		return *this;
	}
	template<typename X, typename Y>
	Gnuplot& plot_2(   //  type has [] and size()
			const X& x, //
			const Y& y, //
			const std::string &str = "") {  //
		if (x.size() != y.size()) {
			std::cerr << " >Warning! The containers' size are not equal. \n";
			std::cerr << " >Warning! x =" << x.size() << " y =" << y.size
					<< " \n";
		}
		// inline data
		std::ostringstream sst;
		//
		sst << "plot \"-\" using 1:2 " << str;
		cmd(sst.str());
		sst.str("");
		typename X::const_iterator iterx = x.begin();
		typename Y::const_iterator itery = y.begin();
		if (x.size() >= y.size()) {
			for (; itery != y.end();) {
				sst << (*iterx) << " " << (*itery);
				sst << "\n";
				cmd(sst.str());
				sst.str("");
				iterx++;
				itery++;
			}
		} else {
			for (; iterx != x.end();) {
				sst << (*iterx) << " " << (*itery);
				sst << "\n";
				cmd(sst.str());
				sst.str("");
				iterx++;
				itery++;
			}
		}
		cmd("e\n");
		return *this;
	}

	Gnuplot& plot(const Gnuplot_actor& actor) {
		if (actor.empty()) {
			std::cerr << " >Warning! The Gnuplot actor is empty! \n";
			return *this;
		}
		// inline data
		std::ostringstream sst;
		//
		sst << "plot \"-\" " << actor._pcmd;
		cmd(sst.str());
		sst.str("");
		cmd("\n");
		for (std::list<std::string>::const_iterator iter = actor._data.begin();
				iter != actor._data.end(); ++iter) {
			sst << (*iter);
			cmd(sst.str());
			sst.str("");
			cmd("\n");
		}
		cmd("e\n");
		return *this;
	}

	Gnuplot& plot(const std::list<std::shared_ptr<Gnuplot_actor> >& lga) {
		if (lga.empty()) {
			std::cerr << " >Warning! The Gnuplot actor is empty! \n";
			return *this;
		}
		std::ostringstream ss;
		ss << "plot ";
		for (std::list<std::shared_ptr<Gnuplot_actor> >::const_iterator iter =
				lga.begin(); iter != lga.end(); ++iter) {
			const std::shared_ptr<Gnuplot_actor> spa = (*iter);
			if (spa->empty_style()) {
				ss << "\"-\" " << spa->command() << "with lines lw 1";
			} else {
				ss << "\"-\" " << spa->command() << spa->style();
			}

			if (lga.size() >= 2 && (iter != (--lga.end()))) {
				ss << ",\\\n";
			}
		}
		cmd(ss.str() + "\n");
		ss.str("");
		for (std::list<std::shared_ptr<Gnuplot_actor> >::const_iterator iter =
				lga.begin(); iter != lga.end(); ++iter) {
			const std::shared_ptr<Gnuplot_actor> spa = (*iter);
			output_inline_data((*spa));
		}
		return *this;
	}

	Gnuplot& plot(const std::list<Gnuplot_actor>& lga) {
		if (lga.empty()) {
			std::cerr << " >Warning! The Gnuplot actor is empty! \n";
			return *this;
		}
		std::ostringstream ss;
		ss << "plot ";
		for (std::list<Gnuplot_actor>::const_iterator iter = lga.begin();
				iter != lga.end(); ++iter) {
			if (iter->empty_style()) {
				ss << "\"-\" " << iter->command() << "with lines lw 1";
			} else {
				ss << "\"-\" " << iter->command() << iter->style();
			}

			if (lga.size() >= 2 && (iter != (--lga.end()))) {
				ss << ",\\\n";
			}
		}
		cmd(ss.str() + "\n");
		ss.str("");
		for (std::list<Gnuplot_actor>::const_iterator iter = lga.begin();
				iter != lga.end(); ++iter) {
			output_inline_data((*iter));
		}
		return *this;
	}

	Gnuplot& splot(const std::list<Gnuplot_actor>& lga) {
		if (lga.empty()) {
			std::cerr << " >Warning! The Gnuplot actor is empty! \n";
			return *this;
		}
		std::ostringstream ss;
		ss << "splot ";
		for (std::list<Gnuplot_actor>::const_iterator iter = lga.begin();
				iter != lga.end(); ++iter) {
			if (iter->empty_style()) {
				ss << "\"-\" " << iter->command() << "with lines lw 1";
			} else {
				ss << "\"-\" " << iter->command() << iter->style();
			}

			if (lga.size() >= 2 && (iter != (--lga.end()))) {
				ss << ",\\\n";
			}
		}
		cmd(ss.str() + "\n");
		ss.str("");
		for (std::list<Gnuplot_actor>::const_iterator iter = lga.begin();
				iter != lga.end(); ++iter) {
			output_inline_data((*iter));
		}
		return *this;
	}
	Gnuplot& splot(const Gnuplot_actor& actor) {
		if (actor.empty()) {
			std::cerr << " >Warning! The Gnuplot actor is empty! \n";
			return *this;
		}
		// inline data
		std::ostringstream sst;
		//
		sst << "splot \"-\" " << actor._pcmd;
		cmd(sst.str());
		sst.str("");
		cmd("\n");
		for (std::list<std::string>::const_iterator iter = actor._data.begin();
				iter != actor._data.end(); ++iter) {
			sst << (*iter);
			cmd(sst.str());
			sst.str("");
			cmd("\n");
		}
		cmd("e\n");
		return *this;
	}

	Gnuplot& splot(const std::list<std::shared_ptr<Gnuplot_actor> >& lga) {
		if (lga.empty()) {
			std::cerr << " >Warning! The Gnuplot actor is empty! \n";
			return *this;
		}
		std::ostringstream ss;
		ss << "splot ";
		for (std::list<std::shared_ptr<Gnuplot_actor> >::const_iterator iter =
				lga.begin(); iter != lga.end(); ++iter) {
			const std::shared_ptr<Gnuplot_actor> spa = (*iter);
			if (spa->empty_style()) {
				ss << "\"-\" " << spa->command() << "with lines lw 1";
			} else {
				ss << "\"-\" " << spa->command() << spa->style();
		}

		if (lga.size() >= 2 && (iter != (--lga.end()))) {
				ss << ",\\\n";
			}
		}
		cmd(ss.str() + "\n");
		ss.str("");
		for (std::list<std::shared_ptr<Gnuplot_actor> >::const_iterator iter =
				lga.begin(); iter != lga.end(); ++iter) {
			const std::shared_ptr<Gnuplot_actor> spa = (*iter);
			output_inline_data((*spa));
		}
		return *this;
	}

	Gnuplot& output_inline_data(const Gnuplot_actor& actor) {
		std::ostringstream sst;
		for (std::list<std::string>::const_iterator iter = actor._data.begin();
				iter != actor._data.end(); ++iter) {
			sst << (*iter);
			cmd(sst.str());
			sst.str("");
		}
		cmd("e\n");
		return *this;
	}

	// inner actors
	Gnuplot& add(spActor actor){
		this->_actors.push_back(actor);
		return *this;
	}
	Gnuplot& clear(){
		this->_actors.clear();
		return *this;
	}

	Gnuplot& splot(){
		this->splot(this->_actors);
		this->clear();
		return *this;
	}

	Gnuplot& plot(){
		this->plot(this->_actors);
		this->clear();
		return *this;
	}

	Gnuplot& save_cmd(const std::string& filename){
		this->cmd("save \'" + filename + "\'");
		return *this;
	}
}
;

class GnuplotActorMaker{
public:
    typedef std::shared_ptr<carpio::Gnuplot_actor> spActor;

public:
    GnuplotActorMaker(){};

    template<typename X, typename Y>
    spActor array_xy(const X& x, const Y& y,
            const std::string &pcmd = "using 1:2 title \"\" ",
            const std::string& scmd = "") {
        auto actor = _make_spactor(pcmd, scmd);
        ASSERT(x.size() == y.size());
        typename X::const_iterator xiter = x.begin();
        typename X::const_iterator yiter = y.begin();
        for (; xiter != x.end();) {
            std::ostringstream sst;
            sst << (*xiter) << " " << (*yiter);
            actor->data().push_back(sst.str());
            xiter++;
            yiter++;
        }
        return actor;
    }

    template<typename Container>
    spActor matrix_xy(
            const Container& con, // matrix like data structure
            const St& c1, // column index
            const St& c2, // column index
            const std::string &pcmd = "using 1:2 title \"\" ",
            const std::string& scmd = "") {
        auto actor = _make_spactor(pcmd, scmd);
        for (auto& row : con) {
            std::ostringstream sst;
            sst << row[c1] << " " << row[c2];
            actor->data().push_back(sst.str());
        }
        return actor;
    }

protected:
	spActor _make_spactor(
	    const std::string &pcmd = "using 1:2 title \"\" ",
        const std::string& scmd = ""){
	    spActor actor    = spActor(new Gnuplot_actor());
	    actor->command() = pcmd;
	    actor->style()   = scmd;
	    return actor;
	}
};

//------------------------------------------------------------------------------
//int GnuplotShow(const std::list<Gnuplot_actor>& lga);
//int GnuplotShow(Gnuplot&, const std::list<Gnuplot_actor>& lga);
namespace GnuplotActor {

typedef std::shared_ptr<carpio::Gnuplot_actor> spActor;
typedef std::list<spActor> list_spActor;

template<typename X, typename Y>
spActor XY(const X& x, const Y& y,
        const std::string &pcmd = "using 1:2 title \"\" ",
        const std::string& scmd = "") {
    spActor actor = spActor(new Gnuplot_actor());
    actor->command() = pcmd;
    actor->style() = scmd;
    ASSERT(x.size() == y.size());
    typename X::const_iterator xiter = x.begin();
    typename X::const_iterator yiter = y.begin();
    for (; xiter != x.end();) {
        std::ostringstream sst;
        sst << (*xiter) << " " << (*yiter);
        actor->data().push_back(sst.str());
        xiter++;
        yiter++;
    }
//	actor->data().push_back("");
    return actor;
}

template<typename Container>
spActor XY(
        const Container& con, // matrix like data structure
        const St& c1, // column index
        const St& c2, // column index
        const std::string &pcmd = "using 1:2 title \"\" ",
        const std::string& scmd = "") {
    spActor actor = spActor(new Gnuplot_actor());
    actor->command() = pcmd;
    actor->style() = scmd;
    for (auto& row : con) {
        std::ostringstream sst;
        sst << row[c1] << " " << row[c2];
        actor->data().push_back(sst.str());
    }
    return actor;
}

}

}

#endif
