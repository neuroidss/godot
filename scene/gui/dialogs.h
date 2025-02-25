/*************************************************************************/
/*  dialogs.h                                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef DIALOGS_H
#define DIALOGS_H

#include "box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/panel.h"
#include "scene/gui/popup.h"
#include "scene/gui/texture_button.h"
#include "scene/main/window.h"

class LineEdit;

class AcceptDialog : public Window {
	GDCLASS(AcceptDialog, Window);

	Window *parent_visible = nullptr;
	Panel *bg = nullptr;
	HBoxContainer *hbc = nullptr;
	Label *label = nullptr;
	Button *ok = nullptr;
	bool hide_on_ok = true;
	bool close_on_escape = true;

	struct ThemeCache {
		Ref<StyleBox> panel_style;
		int margin = 0;
		int button_margin = 0;
	} theme_cache;

	void _custom_action(const String &p_action);
	void _update_child_rects();

	static bool swap_cancel_ok;

	void _input_from_window(const Ref<InputEvent> &p_event);
	void _parent_focused();

protected:
	virtual Size2 _get_contents_minimum_size() const override;
	virtual void _update_theme_item_cache() override;

	void _notification(int p_what);
	static void _bind_methods();
	virtual void ok_pressed() {}
	virtual void cancel_pressed() {}
	virtual void custom_action(const String &) {}

	// Not private since used by derived classes signal.
	void _text_submitted(const String &p_text);
	void _ok_pressed();
	void _cancel_pressed();

public:
	Label *get_label() { return label; }
	static void set_swap_cancel_ok(bool p_swap);

	void register_text_enter(Control *p_line_edit);

	Button *get_ok_button() { return ok; }
	Button *add_button(const String &p_text, bool p_right = false, const String &p_action = "");
	Button *add_cancel_button(const String &p_cancel = "");
	void remove_button(Control *p_button);

	void set_hide_on_ok(bool p_hide);
	bool get_hide_on_ok() const;

	void set_close_on_escape(bool p_enable);
	bool get_close_on_escape() const;

	void set_text(String p_text);
	String get_text() const;

	void set_autowrap(bool p_autowrap);
	bool has_autowrap();

	void set_ok_button_text(String p_ok_button_text);
	String get_ok_button_text() const;

	AcceptDialog();
	~AcceptDialog();
};

class ConfirmationDialog : public AcceptDialog {
	GDCLASS(ConfirmationDialog, AcceptDialog);
	Button *cancel = nullptr;

protected:
	static void _bind_methods();

public:
	Button *get_cancel_button();

	void set_cancel_button_text(String p_cancel_button_text);
	String get_cancel_button_text() const;

	ConfirmationDialog();
};

#endif // DIALOGS_H
