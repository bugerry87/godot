/*************************************************************************/
/*  tree.h                                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
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
#ifndef TREE_H
#define TREE_H

#include "scene/gui/control.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/scroll_bar.h"
#include "scene/gui/slider.h"
#include "core/helper/value_evaluator.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/


class Tree;

class TreeItem : public Object {

	OBJ_TYPE(TreeItem,Object);
public:

	enum TreeCellMode {

		CELL_MODE_STRING, ///< just a string
		CELL_MODE_CHECK, ///< string + check
		CELL_MODE_RANGE, ///< Contains a range
		CELL_MODE_RANGE_EXPRESSION, ///< Contains a range
		CELL_MODE_ICON, ///< Contains a icon, not editable
		CELL_MODE_CUSTOM, ///< Contains a custom value, show a string, and an edit button
	};

private:
friend class Tree;


	struct Cell {

		TreeCellMode mode;

		Ref<Texture> icon;
		Rect2i icon_region;
		String text;
		String suffix;
		double min,max,step,val;
		int icon_max_w;
		bool expr;
		bool checked;
		bool editable;
		bool selected;
		bool selectable;
		bool custom_color;
		Color color;
		bool custom_bg_color;
		bool custom_bg_outline;
		Color bg_color;

		Variant meta;
		String tooltip;

		ObjectID custom_draw_obj;
		StringName custom_draw_callback;

		struct Button {
			int id;
			bool disabled;
			Ref<Texture> texture;
			Button() { id=0; disabled=false; }
		};

		Vector< Button > buttons;

		Cell() {

			custom_draw_obj=0;
			mode=TreeItem::CELL_MODE_STRING;
			min=0;
			max=100;
			step=1;
			val=0;
			checked=false;
			editable=false;
			selected=false;
			selectable=true;
			custom_color=false;
			custom_bg_color=false;
			expr=false;
			icon_max_w=0;
		}


		Size2 get_icon_size() const;
		void draw_icon(const RID& p_where, const Point2& p_pos, const Size2& p_size=Size2()) const;

	};

	Vector<Cell> cells;

	bool collapsed; // wont show childs

	TreeItem *parent; // parent item
	TreeItem *next; // next in list
	TreeItem *childs; //child items
	Tree *tree; //tree (for reference)



	TreeItem(Tree *p_tree);


	void _changed_notify(int p_cell);
	void _changed_notify();
	void _cell_selected(int p_cell);
	void _cell_deselected(int p_cell);
protected:

	static void _bind_methods();
	//bind helpers
	Dictionary _get_range_config( int p_column ) {
		Dictionary d;
		double min,max,step;
		get_range_config(p_column,min,max,step);
		d["min"]=min;
		d["max"]=max;
		d["step"]=step;
		d["expr"]=false;

		return d;
	}
	void _remove_child(Object *p_child) { remove_child( p_child->cast_to<TreeItem>() ); }
public:

	/* cell mode */
	void set_cell_mode( int p_column, TreeCellMode p_mode );
	TreeCellMode get_cell_mode( int p_column ) const;

	/* check mode */
	void set_checked(int p_column,bool p_checked);
	bool is_checked(int p_column) const;

	void set_text(int p_column,String p_text);
	String get_text(int p_column) const;

	void set_suffix(int p_column,String p_suffix);
	String get_suffix(int p_column) const;

	void set_icon(int p_column,const Ref<Texture>& p_icon);
	Ref<Texture> get_icon(int p_column) const;

	void set_icon_region(int p_column,const Rect2& p_icon_region);
	Rect2 get_icon_region(int p_column) const;

	void set_icon_max_width(int p_column,int p_max);
	int get_icon_max_width(int p_column) const;

	void add_button(int p_column,const Ref<Texture>& p_button,int p_id=-1,bool p_disabled=false);
	int get_button_count(int p_column) const;
	Ref<Texture> get_button(int p_column,int p_idx) const;
	int get_button_id(int p_column,int p_idx) const;
	void erase_button(int p_column,int p_idx);
	int get_button_by_id(int p_column,int p_id) const;
	bool is_button_disabled(int p_column,int p_idx) const;
	void set_button(int p_column,int p_idx,const Ref<Texture>& p_button);

	/* range works for mode number or mode combo */

	void set_range(int p_column,double p_value);
	double get_range(int p_column) const;

	void set_range_config(int p_column,double p_min,double p_max,double p_step,bool p_exp=false);
	void get_range_config(int p_column,double& r_min,double& r_max,double &r_step) const;
	bool is_range_exponential(int p_column) const;

	void set_metadata(int p_column,const Variant& p_meta);
	Variant get_metadata(int p_column) const;

	void set_custom_draw(int p_column,Object *p_object,const StringName& p_callback);

	void set_collapsed(bool p_collapsed);
	bool is_collapsed();

	TreeItem *get_prev();
	TreeItem *get_next();
	TreeItem *get_parent();
	TreeItem *get_children();

	TreeItem *get_prev_visible();
	TreeItem *get_next_visible();

	void remove_child(TreeItem *p_item);

	void set_selectable(int p_column,bool p_selectable);
	bool is_selectable(int p_column) const;

	bool is_selected(int p_column);
	void select(int p_column);
	void deselect(int p_column);
	void set_as_cursor(int p_column);

	void set_editable(int p_column,bool p_editable);
	bool is_editable(int p_column);

	void set_custom_color(int p_column,const Color& p_color);
	Color get_custom_color(int p_column) const;
	void clear_custom_color(int p_column);

	void set_custom_bg_color(int p_column, const Color& p_color, bool p_bg_outline=false);
	void clear_custom_bg_color(int p_column);
	Color get_custom_bg_color(int p_column) const;

	void set_tooltip(int p_column, const String& p_tooltip);
	String get_tooltip(int p_column) const;


	void clear_children();

	void move_to_top();
	void move_to_bottom();

	~TreeItem();

};


VARIANT_ENUM_CAST( TreeItem::TreeCellMode );


class Tree : public Control {

	OBJ_TYPE( Tree, Control );
public:
	enum SelectMode {
            SELECT_SINGLE,
            SELECT_ROW,
            SELECT_MULTI
	};

	enum DropModeFlags {
		DROP_MODE_DISABLED=0,
		DROP_MODE_ON_ITEM=1,
		DROP_MODE_INBETWEEN=2
	};

private:
friend class TreeItem;

	TreeItem *root;
	TreeItem *popup_edited_item;
	TreeItem *selected_item;
	TreeItem *edited_item;

	TreeItem *drop_mode_over;
	int drop_mode_section;

	TreeItem *single_select_defer;
	int single_select_defer_column;

	int pressed_button;
	bool pressing_for_editor;
	String pressing_for_editor_text;
	Vector2 pressing_pos;
	Rect2 pressing_item_rect;

	float range_drag_base;
	bool range_drag_enabled;
	Vector2 range_drag_capture_pos;


	//TreeItem *cursor_item;
	//int cursor_column;

	Rect2 custom_popup_rect;
	int edited_col;
	int selected_col;
	int popup_edited_item_col;
	bool hide_root;
	SelectMode select_mode;

	int blocked;

	int drop_mode_flags;

	struct ColumnInfo {

		int min_width;
		bool expand;
		String title;
		ColumnInfo() { min_width=1; expand=true; }
	};

	bool show_column_titles;
	LineEdit *text_editor;
	HSlider *value_editor;
	bool updating_value_editor;
	uint32_t focus_in_id;
	PopupMenu *popup_menu;

	Vector<ColumnInfo> columns;

	Timer *range_click_timer;
	TreeItem *range_item_last;
	bool range_up_last;
	void _range_click_timeout();

	int compute_item_height(TreeItem *p_item) const;
	int get_item_height(TreeItem *p_item) const;
//	void draw_item_text(String p_text,const Ref<Texture>& p_icon,int p_icon_max_w,bool p_tool,Rect2i p_rect,const Color& p_color);
	void draw_item_rect(const TreeItem::Cell& p_cell,const Rect2i& p_rect,const Color& p_color);
	int draw_item(const Point2i& p_pos,const Point2& p_draw_ofs, const Size2& p_draw_size,TreeItem *p_item);
	void select_single_item(TreeItem *p_selected,TreeItem *p_current,int p_col,TreeItem *p_prev=NULL,bool *r_in_range=NULL,bool p_force_deselect=false);
	int propagate_mouse_event(const Point2i &p_pos,int x_ofs,int y_ofs,bool p_doubleclick,TreeItem *p_item,int p_button,const InputModifierState& p_mod);
	void text_editor_enter(String p_text);
	void _text_editor_modal_close();
	void value_editor_changed(double p_value);

	void popup_select(int p_option);

	void _input_event(InputEvent p_event);
	void _notification(int p_what);

	Size2 get_minimum_size() const;

	void item_edited(int p_column,TreeItem *p_item);
	void item_changed(int p_column,TreeItem *p_item);
	void item_selected(int p_column,TreeItem *p_item);
	void item_deselected(int p_column,TreeItem *p_item);

	void propagate_set_columns(TreeItem *p_item);

	struct Cache {

		Ref<Font> font;
		Ref<Font> tb_font;
		Ref<StyleBox> bg;
		Ref<StyleBox> selected;
		Ref<StyleBox> selected_focus;
		Ref<StyleBox> cursor;
		Ref<StyleBox> cursor_unfocus;
		Ref<StyleBox> button_pressed;
		Ref<StyleBox> title_button;
		Ref<StyleBox> title_button_hover;
		Ref<StyleBox> title_button_pressed;
		Color title_button_color;

		Ref<Texture> checked;
		Ref<Texture> unchecked;
		Ref<Texture> arrow_collapsed;
		Ref<Texture> arrow;
		Ref<Texture> select_arrow;
		Ref<Texture> updown;

		Color font_color;
		Color font_color_selected;
		Color guide_color;
		Color drop_position_color;
		Color relationship_line_color;

		int hseparation;
		int vseparation;
		int item_margin;
		int guide_width;
		int button_margin;
		Point2 offset;
		int draw_relationship_lines;
		int scroll_border;
		int scroll_speed;

		enum ClickType {
			CLICK_NONE,
			CLICK_TITLE,
			CLICK_BUTTON,

		};

		ClickType click_type;
		ClickType hover_type;
		int click_index;
		int click_id;
		TreeItem *click_item;
		int click_column;
		int hover_index;

	} cache;


	int _get_title_button_height() const;

	void _scroll_moved(float p_value);
	HScrollBar *h_scroll;
	VScrollBar *v_scroll;

	Size2 get_internal_min_size() const;
	void update_cache();
	void update_scrollbars();

	Rect2 search_item_rect(TreeItem *p_from, TreeItem *p_item);
	//Rect2 get_item_rect(TreeItem *p_item);
	uint64_t last_keypress;
	String incr_search;
	bool cursor_can_exit_tree;
	void _do_incr_search(const String& p_add);

	TreeItem* _search_item_text(TreeItem *p_at, const String& p_find,int *r_col,bool p_selectable,bool p_backwards=false);

	TreeItem* _find_item_at_pos(TreeItem *p_current, const Point2& p_pos, int& r_column, int &h, int &section) const;

/*	float drag_speed;
	float drag_accum;

	float last_drag_accum;
	float last_drag_time;
	float time_since_motion;*/

	bool delayed_text_editor;
	uint64_t first_selection_time;

	float drag_speed;
	float drag_from;
	float drag_accum;
	Vector2 last_speed;
	bool drag_touching;
	bool drag_touching_deaccel;
	bool click_handled;
	bool allow_rmb_select;
	bool scrolling;

	bool force_select_on_already_selected;

	bool hide_folding;

	ValueEvaluator *evaluator;

	int _count_selected_items(TreeItem* p_from) const;

protected:
	static void _bind_methods();

	//bind helpers
	Object* _create_item(Object *p_parent) { return create_item(p_parent->cast_to<TreeItem>() ); }
	TreeItem *_get_next_selected(Object *p_item) { return get_next_selected(p_item->cast_to<TreeItem>() ); }
	Rect2 _get_item_rect(Object *p_item,int p_column) const { return get_item_rect(p_item->cast_to<TreeItem>(),p_column ); }


public:

	virtual String get_tooltip(const Point2& p_pos) const;

	TreeItem* get_item_at_pos(const Point2& p_pos) const;
	int get_column_at_pos(const Point2& p_pos) const;
	int get_drop_section_at_pos(const Point2& p_pos) const;

	void clear();

	TreeItem* create_item(TreeItem *p_parent=0);
	TreeItem* get_root();
	TreeItem* get_last_item();

	void set_column_min_width(int p_column,int p_min_width);
	void set_column_expand(int p_column,bool p_expand);
	int get_column_width(int p_column) const;

	void set_hide_root(bool p_eanbled);
	TreeItem *get_next_selected( TreeItem* p_item);
	TreeItem *get_selected() const;
	int get_selected_column() const;
	int get_pressed_button() const;
	void set_select_mode(SelectMode p_mode);

	void set_columns(int p_columns);
	int get_columns() const;

	void set_column_title(int p_column,const String& p_title);
	String get_column_title(int p_column) const;

	void set_column_titles_visible(bool p_show);
	bool are_column_titles_visible() const;

	TreeItem *get_edited() const;
	int get_edited_column() const;

	void ensure_cursor_is_visible();

	Rect2 get_custom_popup_rect() const;

	int get_item_offset(TreeItem *p_item) const;
	Rect2 get_item_rect(TreeItem *p_item,int p_column=-1) const;
	bool edit_selected();

	TreeItem* search_item_text(const String& p_find,int *r_col=NULL,bool p_selectable=false);

	Point2 get_scroll() const;

	void set_cursor_can_exit_tree(bool p_enable);
	bool can_cursor_exit_tree() const;

	VScrollBar *get_vscroll_bar() { return v_scroll; }

	void set_hide_folding(bool p_hide);
	bool is_folding_hidden() const;

	void set_drop_mode_flags(int p_flags);
	int get_drop_mode_flags() const;

	void set_single_select_cell_editing_only_when_already_selected(bool p_enable);
	bool get_single_select_cell_editing_only_when_already_selected() const;

	void set_allow_rmb_select(bool p_allow);
	bool get_allow_rmb_select() const;

	void set_value_evaluator(ValueEvaluator *p_evaluator);

	void set_delayed_text_editor(bool enabled);
	bool is_delayed_text_editor_enabled() const;

	Tree();
	~Tree();

};

VARIANT_ENUM_CAST( Tree::SelectMode );
#endif
