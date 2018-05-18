#pragma once

#include "common.h"
#include "UI/WxBasicControls.h"

class ModifyOffsetsDialog : public wxDialog
{
public:
	ModifyOffsetsDialog();
	~ModifyOffsetsDialog() = default;

	point2_t getOffset();
	int      getAlignType();
	bool     autoOffset();
	bool     relativeOffset();
	bool     xOffChange();
	bool     yOffChange();
	point2_t calculateOffsets(int xoff, int yoff, int width, int height);

private:
	wxChoice*      combo_aligntype_;
	wxTextCtrl*    entry_xoff_;
	wxTextCtrl*    entry_yoff_;
	wxCheckBox*    cbox_relative_;
	wxRadioButton* opt_set_;
	wxRadioButton* opt_auto_;
};
