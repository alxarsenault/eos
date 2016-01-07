#ifndef __EOS_TERMINAL_PARSER_H__
#define __EOS_TERMINAL_PARSER_H__

#include "eosTerminalType.h"
#include <axLib/axLib.h>

namespace eos {
class Parser {
public:
	Parser();

	void ParseOutput(const std::string& out_str);

	const ax::StringVector& GetOutput() const
	{
		return _terminal_output;
	}

	inline ColorMap& GetColorMap()
	{
		return _colors;
	}
	
	inline BoldMap& GetBoldMap()
	{
		return _bold_map;
	}

	ax::Utils::Point2D<std::size_t> GetCursor() const
	{
		return _cursor_write_pos;
	}

private:
	int _tab_width = 8;
	ax::Size _terminal_size = ax::Size(80, 24);
	ax::Utils::Point2D<std::size_t> _cursor_write_pos;

	ax::StringVector _terminal_output;
	ColorMap _colors;
	BoldMap _bold_map;

	/// @param esc_pos Index of the escape character '\033'.
	/// @return Index position to continue parsing.
	std::size_t ParseEscapceSequence(const std::string& out_str, const std::size_t& esc_pos);

	std::string AppendAlignedTab(const std::size_t& tab_position) const;

	std::size_t GetEscapeSegment(
		const std::string& out_str, const std::size_t& esc_pos, ax::StringVector& ansi_esc);

	void ProcessEscapeSegment(const ax::StringVector& ansi_esc);

	///	Calls the graphics functions specified by the following values. These specified functions remain
	/// active until the next occurrence of this escape sequence. Graphics mode changes the colors and
	/// attributes of text (such as bold and underline) displayed on the screen.
	///
	///	Text attributes
	///	0 : All attributes off
	///	1 : Bold on
	///	4 : Underscore (on monochrome display adapter only)
	///	5 : Blink on
	///	7 : Reverse video on
	///	8 : Concealed on
	///
	///	Foreground colors
	///	30 : Black
	///	31 : Red
	///	32 : Green
	///	33 : Yellow
	///	34 : Blue
	///	35 : Magenta
	///	36 : Cyan
	///	37 : White
	///
	///	Background colors
	///	40 : Black
	///	41 : Red
	///	42 : Green
	///	43 : Yellow
	///	44 : Blue
	///	45 : Magenta
	///	46 : Cyan
	///	47 : White
	///
	///	Parameters 30 through 47 meet the ISO 6429 standard.
	void ProcessGraphicMode(const ax::StringVector& ansi_esc);

	// Clears the screen and moves the cursor to the home position (line 0, column 0).
	void ClearScreen();
};
}

#endif // __EOS_TERMINAL_PARSER_H__