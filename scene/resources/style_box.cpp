/*************************************************************************/
/*  style_box.cpp                                                        */
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

#include "style_box.h"

#include "scene/main/canvas_item.h"

#include <limits.h>

float StyleBox::get_style_margin(Side p_side) const {
	float ret;
	if (GDVIRTUAL_REQUIRED_CALL(_get_style_margin, p_side, ret)) {
		return ret;
	}
	return 0;
}
bool StyleBox::test_mask(const Point2 &p_point, const Rect2 &p_rect) const {
	bool ret;
	if (GDVIRTUAL_CALL(_test_mask, p_point, p_rect, ret)) {
		return ret;
	}

	return true;
}

void StyleBox::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	if (GDVIRTUAL_REQUIRED_CALL(_draw, p_canvas_item, p_rect)) {
		return;
	}
}

void StyleBox::set_default_margin(Side p_side, float p_value) {
	ERR_FAIL_INDEX((int)p_side, 4);

	margin[p_side] = p_value;
	emit_changed();
}

float StyleBox::get_default_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);

	return margin[p_side];
}

float StyleBox::get_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);

	if (margin[p_side] < 0) {
		return get_style_margin(p_side);
	} else {
		return margin[p_side];
	}
}

CanvasItem *StyleBox::get_current_item_drawn() const {
	return CanvasItem::get_current_item_drawn();
}

Size2 StyleBox::get_minimum_size() const {
	return Size2(get_margin(SIDE_LEFT) + get_margin(SIDE_RIGHT), get_margin(SIDE_TOP) + get_margin(SIDE_BOTTOM));
}

Point2 StyleBox::get_offset() const {
	return Point2(get_margin(SIDE_LEFT), get_margin(SIDE_TOP));
}

Size2 StyleBox::get_center_size() const {
	Size2 ret;
	if (GDVIRTUAL_CALL(_get_center_size, ret)) {
		return ret;
	}

	return Size2();
}

Rect2 StyleBox::get_draw_rect(const Rect2 &p_rect) const {
	Rect2 ret;
	if (GDVIRTUAL_CALL(_get_draw_rect, p_rect, ret)) {
		return ret;
	}
	return p_rect;
}

void StyleBox::_bind_methods() {
	ClassDB::bind_method(D_METHOD("test_mask", "point", "rect"), &StyleBox::test_mask);

	ClassDB::bind_method(D_METHOD("set_default_margin", "margin", "offset"), &StyleBox::set_default_margin);
	ClassDB::bind_method(D_METHOD("get_default_margin", "margin"), &StyleBox::get_default_margin);

	ClassDB::bind_method(D_METHOD("get_margin", "margin"), &StyleBox::get_margin);
	ClassDB::bind_method(D_METHOD("get_minimum_size"), &StyleBox::get_minimum_size);
	ClassDB::bind_method(D_METHOD("get_center_size"), &StyleBox::get_center_size);
	ClassDB::bind_method(D_METHOD("get_offset"), &StyleBox::get_offset);
	ClassDB::bind_method(D_METHOD("get_current_item_drawn"), &StyleBox::get_current_item_drawn);

	ClassDB::bind_method(D_METHOD("draw", "canvas_item", "rect"), &StyleBox::draw);

	ADD_GROUP("Content Margins", "content_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_left", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_default_margin", "get_default_margin", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_top", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_default_margin", "get_default_margin", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_right", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_default_margin", "get_default_margin", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_bottom", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_default_margin", "get_default_margin", SIDE_BOTTOM);

	GDVIRTUAL_BIND(_get_style_margin, "side")
	GDVIRTUAL_BIND(_test_mask, "point", "rect")
	GDVIRTUAL_BIND(_get_center_size)
	GDVIRTUAL_BIND(_get_draw_rect, "rect")
	GDVIRTUAL_BIND(_draw, "to_canvas_item", "rect")
}

StyleBox::StyleBox() {
	for (int i = 0; i < 4; i++) {
		margin[i] = -1;
	}
}

void StyleBoxTexture::set_texture(Ref<Texture2D> p_texture) {
	if (texture == p_texture) {
		return;
	}
	texture = p_texture;
	if (p_texture.is_null()) {
		region_rect = Rect2(0, 0, 0, 0);
	} else {
		region_rect = Rect2(Point2(), texture->get_size());
	}
	emit_changed();
}

Ref<Texture2D> StyleBoxTexture::get_texture() const {
	return texture;
}

void StyleBoxTexture::set_margin_size(Side p_side, float p_size) {
	ERR_FAIL_INDEX((int)p_side, 4);

	margin[p_side] = p_size;
	emit_changed();
}

float StyleBoxTexture::get_margin_size(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);

	return margin[p_side];
}

float StyleBoxTexture::get_style_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);

	return margin[p_side];
}

Rect2 StyleBoxTexture::get_draw_rect(const Rect2 &p_rect) const {
	return p_rect.grow_individual(expand_margin[SIDE_LEFT], expand_margin[SIDE_TOP], expand_margin[SIDE_RIGHT], expand_margin[SIDE_BOTTOM]);
}

void StyleBoxTexture::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	if (texture.is_null()) {
		return;
	}

	Rect2 rect = p_rect;
	Rect2 src_rect = region_rect;

	texture->get_rect_region(rect, src_rect, rect, src_rect);

	rect.position.x -= expand_margin[SIDE_LEFT];
	rect.position.y -= expand_margin[SIDE_TOP];
	rect.size.x += expand_margin[SIDE_LEFT] + expand_margin[SIDE_RIGHT];
	rect.size.y += expand_margin[SIDE_TOP] + expand_margin[SIDE_BOTTOM];

	RenderingServer::get_singleton()->canvas_item_add_nine_patch(p_canvas_item, rect, src_rect, texture->get_rid(), Vector2(margin[SIDE_LEFT], margin[SIDE_TOP]), Vector2(margin[SIDE_RIGHT], margin[SIDE_BOTTOM]), RS::NinePatchAxisMode(axis_h), RS::NinePatchAxisMode(axis_v), draw_center, modulate);
}

void StyleBoxTexture::set_draw_center(bool p_enabled) {
	draw_center = p_enabled;
	emit_changed();
}

bool StyleBoxTexture::is_draw_center_enabled() const {
	return draw_center;
}

Size2 StyleBoxTexture::get_center_size() const {
	if (texture.is_null()) {
		return Size2();
	}

	return region_rect.size - get_minimum_size();
}

void StyleBoxTexture::set_expand_margin_size(Side p_side, float p_size) {
	ERR_FAIL_INDEX((int)p_side, 4);
	expand_margin[p_side] = p_size;
	emit_changed();
}

void StyleBoxTexture::set_expand_margin_size_individual(float p_left, float p_top, float p_right, float p_bottom) {
	expand_margin[SIDE_LEFT] = p_left;
	expand_margin[SIDE_TOP] = p_top;
	expand_margin[SIDE_RIGHT] = p_right;
	expand_margin[SIDE_BOTTOM] = p_bottom;
	emit_changed();
}

void StyleBoxTexture::set_expand_margin_size_all(float p_expand_margin_size) {
	for (int i = 0; i < 4; i++) {
		expand_margin[i] = p_expand_margin_size;
	}
	emit_changed();
}

float StyleBoxTexture::get_expand_margin_size(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);
	return expand_margin[p_side];
}

void StyleBoxTexture::set_region_rect(const Rect2 &p_region_rect) {
	if (region_rect == p_region_rect) {
		return;
	}

	region_rect = p_region_rect;
	emit_changed();
}

Rect2 StyleBoxTexture::get_region_rect() const {
	return region_rect;
}

void StyleBoxTexture::set_h_axis_stretch_mode(AxisStretchMode p_mode) {
	ERR_FAIL_INDEX((int)p_mode, 3);
	axis_h = p_mode;
	emit_changed();
}

StyleBoxTexture::AxisStretchMode StyleBoxTexture::get_h_axis_stretch_mode() const {
	return axis_h;
}

void StyleBoxTexture::set_v_axis_stretch_mode(AxisStretchMode p_mode) {
	ERR_FAIL_INDEX((int)p_mode, 3);
	axis_v = p_mode;
	emit_changed();
}

StyleBoxTexture::AxisStretchMode StyleBoxTexture::get_v_axis_stretch_mode() const {
	return axis_v;
}

void StyleBoxTexture::set_modulate(const Color &p_modulate) {
	if (modulate == p_modulate) {
		return;
	}
	modulate = p_modulate;
	emit_changed();
}

Color StyleBoxTexture::get_modulate() const {
	return modulate;
}

void StyleBoxTexture::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &StyleBoxTexture::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &StyleBoxTexture::get_texture);

	ClassDB::bind_method(D_METHOD("set_margin_size", "margin", "size"), &StyleBoxTexture::set_margin_size);
	ClassDB::bind_method(D_METHOD("get_margin_size", "margin"), &StyleBoxTexture::get_margin_size);

	ClassDB::bind_method(D_METHOD("set_expand_margin_size", "margin", "size"), &StyleBoxTexture::set_expand_margin_size);
	ClassDB::bind_method(D_METHOD("set_expand_margin_all", "size"), &StyleBoxTexture::set_expand_margin_size_all);
	ClassDB::bind_method(D_METHOD("set_expand_margin_individual", "size_left", "size_top", "size_right", "size_bottom"), &StyleBoxTexture::set_expand_margin_size_individual);
	ClassDB::bind_method(D_METHOD("get_expand_margin_size", "margin"), &StyleBoxTexture::get_expand_margin_size);

	ClassDB::bind_method(D_METHOD("set_region_rect", "region"), &StyleBoxTexture::set_region_rect);
	ClassDB::bind_method(D_METHOD("get_region_rect"), &StyleBoxTexture::get_region_rect);

	ClassDB::bind_method(D_METHOD("set_draw_center", "enable"), &StyleBoxTexture::set_draw_center);
	ClassDB::bind_method(D_METHOD("is_draw_center_enabled"), &StyleBoxTexture::is_draw_center_enabled);

	ClassDB::bind_method(D_METHOD("set_modulate", "color"), &StyleBoxTexture::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &StyleBoxTexture::get_modulate);

	ClassDB::bind_method(D_METHOD("set_h_axis_stretch_mode", "mode"), &StyleBoxTexture::set_h_axis_stretch_mode);
	ClassDB::bind_method(D_METHOD("get_h_axis_stretch_mode"), &StyleBoxTexture::get_h_axis_stretch_mode);

	ClassDB::bind_method(D_METHOD("set_v_axis_stretch_mode", "mode"), &StyleBoxTexture::set_v_axis_stretch_mode);
	ClassDB::bind_method(D_METHOD("get_v_axis_stretch_mode"), &StyleBoxTexture::get_v_axis_stretch_mode);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");

	ADD_GROUP("Margins", "margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "margin_left", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_margin_size", "get_margin_size", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "margin_top", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_margin_size", "get_margin_size", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "margin_right", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_margin_size", "get_margin_size", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "margin_bottom", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_margin_size", "get_margin_size", SIDE_BOTTOM);

	ADD_GROUP("Expand Margins", "expand_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_left", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin_size", "get_expand_margin_size", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_top", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin_size", "get_expand_margin_size", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_right", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin_size", "get_expand_margin_size", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_bottom", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin_size", "get_expand_margin_size", SIDE_BOTTOM);

	ADD_GROUP("Axis Stretch", "axis_stretch_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis_stretch_horizontal", PROPERTY_HINT_ENUM, "Stretch,Tile,Tile Fit"), "set_h_axis_stretch_mode", "get_h_axis_stretch_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis_stretch_vertical", PROPERTY_HINT_ENUM, "Stretch,Tile,Tile Fit"), "set_v_axis_stretch_mode", "get_v_axis_stretch_mode");

	ADD_GROUP("Sub-Region", "region_");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "region_rect", PROPERTY_HINT_NONE, "suffix:px"), "set_region_rect", "get_region_rect");

	ADD_GROUP("Modulate", "modulate_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate_color"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_center"), "set_draw_center", "is_draw_center_enabled");

	BIND_ENUM_CONSTANT(AXIS_STRETCH_MODE_STRETCH);
	BIND_ENUM_CONSTANT(AXIS_STRETCH_MODE_TILE);
	BIND_ENUM_CONSTANT(AXIS_STRETCH_MODE_TILE_FIT);
}

StyleBoxTexture::StyleBoxTexture() {}

StyleBoxTexture::~StyleBoxTexture() {}

////////////////

void StyleBoxFlat::set_bg_color(const Color &p_color) {
	bg_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_bg_color() const {
	return bg_color;
}

void StyleBoxFlat::set_border_color(const Color &p_color) {
	border_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_border_color() const {
	return border_color;
}

void StyleBoxFlat::set_border_width_all(int p_size) {
	border_width[0] = p_size;
	border_width[1] = p_size;
	border_width[2] = p_size;
	border_width[3] = p_size;
	emit_changed();
}

int StyleBoxFlat::get_border_width_min() const {
	return MIN(MIN(border_width[0], border_width[1]), MIN(border_width[2], border_width[3]));
}

void StyleBoxFlat::set_border_width(Side p_side, int p_width) {
	ERR_FAIL_INDEX((int)p_side, 4);
	border_width[p_side] = p_width;
	emit_changed();
}

int StyleBoxFlat::get_border_width(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);
	return border_width[p_side];
}

void StyleBoxFlat::set_border_blend(bool p_blend) {
	blend_border = p_blend;
	emit_changed();
}

bool StyleBoxFlat::get_border_blend() const {
	return blend_border;
}

void StyleBoxFlat::set_corner_radius_all(int radius) {
	for (int i = 0; i < 4; i++) {
		corner_radius[i] = radius;
	}

	emit_changed();
}

void StyleBoxFlat::set_corner_radius_individual(const int radius_top_left, const int radius_top_right, const int radius_bottom_right, const int radius_bottom_left) {
	corner_radius[0] = radius_top_left;
	corner_radius[1] = radius_top_right;
	corner_radius[2] = radius_bottom_right;
	corner_radius[3] = radius_bottom_left;

	emit_changed();
}

void StyleBoxFlat::set_corner_radius(const Corner p_corner, const int radius) {
	ERR_FAIL_INDEX((int)p_corner, 4);
	corner_radius[p_corner] = radius;
	emit_changed();
}

int StyleBoxFlat::get_corner_radius(const Corner p_corner) const {
	ERR_FAIL_INDEX_V((int)p_corner, 4, 0);
	return corner_radius[p_corner];
}

void StyleBoxFlat::set_expand_margin_size(Side p_side, float p_size) {
	ERR_FAIL_INDEX((int)p_side, 4);
	expand_margin[p_side] = p_size;
	emit_changed();
}

void StyleBoxFlat::set_expand_margin_size_individual(float p_left, float p_top, float p_right, float p_bottom) {
	expand_margin[SIDE_LEFT] = p_left;
	expand_margin[SIDE_TOP] = p_top;
	expand_margin[SIDE_RIGHT] = p_right;
	expand_margin[SIDE_BOTTOM] = p_bottom;
	emit_changed();
}

void StyleBoxFlat::set_expand_margin_size_all(float p_expand_margin_size) {
	for (int i = 0; i < 4; i++) {
		expand_margin[i] = p_expand_margin_size;
	}
	emit_changed();
}

float StyleBoxFlat::get_expand_margin_size(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	return expand_margin[p_side];
}

void StyleBoxFlat::set_draw_center(bool p_enabled) {
	draw_center = p_enabled;
	emit_changed();
}

bool StyleBoxFlat::is_draw_center_enabled() const {
	return draw_center;
}

void StyleBoxFlat::set_skew(Vector2 p_skew) {
	skew = p_skew;
	emit_changed();
}

Vector2 StyleBoxFlat::get_skew() const {
	return skew;
}

void StyleBoxFlat::set_shadow_color(const Color &p_color) {
	shadow_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_shadow_color() const {
	return shadow_color;
}

void StyleBoxFlat::set_shadow_size(const int &p_size) {
	shadow_size = p_size;
	emit_changed();
}

int StyleBoxFlat::get_shadow_size() const {
	return shadow_size;
}

void StyleBoxFlat::set_shadow_offset(const Point2 &p_offset) {
	shadow_offset = p_offset;
	emit_changed();
}

Point2 StyleBoxFlat::get_shadow_offset() const {
	return shadow_offset;
}

void StyleBoxFlat::set_anti_aliased(const bool &p_anti_aliased) {
	anti_aliased = p_anti_aliased;
	emit_changed();
	notify_property_list_changed();
}

bool StyleBoxFlat::is_anti_aliased() const {
	return anti_aliased;
}

void StyleBoxFlat::set_aa_size(const real_t p_aa_size) {
	aa_size = CLAMP(p_aa_size, 0.01, 10);
	emit_changed();
}

real_t StyleBoxFlat::get_aa_size() const {
	return aa_size;
}

void StyleBoxFlat::set_corner_detail(const int &p_corner_detail) {
	corner_detail = CLAMP(p_corner_detail, 1, 20);
	emit_changed();
}

int StyleBoxFlat::get_corner_detail() const {
	return corner_detail;
}

Size2 StyleBoxFlat::get_center_size() const {
	return Size2();
}

inline void set_inner_corner_radius(const Rect2 style_rect, const Rect2 inner_rect, const real_t corner_radius[4], real_t *inner_corner_radius) {
	real_t border_left = inner_rect.position.x - style_rect.position.x;
	real_t border_top = inner_rect.position.y - style_rect.position.y;
	real_t border_right = style_rect.size.width - inner_rect.size.width - border_left;
	real_t border_bottom = style_rect.size.height - inner_rect.size.height - border_top;

	real_t rad;

	// Top left.
	rad = MIN(border_top, border_left);
	inner_corner_radius[0] = MAX(corner_radius[0] - rad, 0);

	// Top right;
	rad = MIN(border_top, border_right);
	inner_corner_radius[1] = MAX(corner_radius[1] - rad, 0);

	// Bottom right.
	rad = MIN(border_bottom, border_right);
	inner_corner_radius[2] = MAX(corner_radius[2] - rad, 0);

	// Bottom left.
	rad = MIN(border_bottom, border_left);
	inner_corner_radius[3] = MAX(corner_radius[3] - rad, 0);
}

inline void draw_ring(Vector<Vector2> &verts, Vector<int> &indices, Vector<Color> &colors, const Rect2 &style_rect, const real_t corner_radius[4],
		const Rect2 &ring_rect, const Rect2 &inner_rect, const Color &inner_color, const Color &outer_color, const int corner_detail, const Vector2 &skew, bool fill_center = false) {
	int vert_offset = verts.size();
	if (!vert_offset) {
		vert_offset = 0;
	}

	int adapted_corner_detail = (corner_radius[0] == 0 && corner_radius[1] == 0 && corner_radius[2] == 0 && corner_radius[3] == 0) ? 1 : corner_detail;

	real_t ring_corner_radius[4];
	set_inner_corner_radius(style_rect, ring_rect, corner_radius, ring_corner_radius);

	// Corner radius center points.
	Vector<Point2> outer_points = {
		ring_rect.position + Vector2(ring_corner_radius[0], ring_corner_radius[0]), //tl
		Point2(ring_rect.position.x + ring_rect.size.x - ring_corner_radius[1], ring_rect.position.y + ring_corner_radius[1]), //tr
		ring_rect.position + ring_rect.size - Vector2(ring_corner_radius[2], ring_corner_radius[2]), //br
		Point2(ring_rect.position.x + ring_corner_radius[3], ring_rect.position.y + ring_rect.size.y - ring_corner_radius[3]) //bl
	};

	real_t inner_corner_radius[4];
	set_inner_corner_radius(style_rect, inner_rect, corner_radius, inner_corner_radius);

	Vector<Point2> inner_points = {
		inner_rect.position + Vector2(inner_corner_radius[0], inner_corner_radius[0]), //tl
		Point2(inner_rect.position.x + inner_rect.size.x - inner_corner_radius[1], inner_rect.position.y + inner_corner_radius[1]), //tr
		inner_rect.position + inner_rect.size - Vector2(inner_corner_radius[2], inner_corner_radius[2]), //br
		Point2(inner_rect.position.x + inner_corner_radius[3], inner_rect.position.y + inner_rect.size.y - inner_corner_radius[3]) //bl
	};
	// Calculate the vertices.
	for (int corner_index = 0; corner_index < 4; corner_index++) {
		for (int detail = 0; detail <= adapted_corner_detail; detail++) {
			for (int inner_outer = 0; inner_outer < 2; inner_outer++) {
				real_t radius;
				Color color;
				Point2 corner_point;
				if (inner_outer == 0) {
					radius = inner_corner_radius[corner_index];
					color = inner_color;
					corner_point = inner_points[corner_index];
				} else {
					radius = ring_corner_radius[corner_index];
					color = outer_color;
					corner_point = outer_points[corner_index];
				}

				const real_t x = radius * (real_t)cos((corner_index + detail / (double)adapted_corner_detail) * (Math_TAU / 4.0) + Math_PI) + corner_point.x;
				const real_t y = radius * (real_t)sin((corner_index + detail / (double)adapted_corner_detail) * (Math_TAU / 4.0) + Math_PI) + corner_point.y;
				const float x_skew = -skew.x * (y - ring_rect.get_center().y);
				const float y_skew = -skew.y * (x - ring_rect.get_center().x);
				verts.push_back(Vector2(x + x_skew, y + y_skew));
				colors.push_back(color);
			}
		}
	}

	int ring_vert_count = verts.size() - vert_offset;

	// Fill the indices and the colors for the border.
	for (int i = 0; i < ring_vert_count; i++) {
		indices.push_back(vert_offset + ((i + 0) % ring_vert_count));
		indices.push_back(vert_offset + ((i + 2) % ring_vert_count));
		indices.push_back(vert_offset + ((i + 1) % ring_vert_count));
	}

	if (fill_center) {
		//Fill the indices and the colors for the center.
		for (int index = 0; index < ring_vert_count / 2; index += 2) {
			int i = index;
			// Polygon 1.
			indices.push_back(vert_offset + i);
			indices.push_back(vert_offset + ring_vert_count - 4 - i);
			indices.push_back(vert_offset + i + 2);
			// Polygon 2.
			indices.push_back(vert_offset + i);
			indices.push_back(vert_offset + ring_vert_count - 2 - i);
			indices.push_back(vert_offset + ring_vert_count - 4 - i);
		}
	}
}

inline void adapt_values(int p_index_a, int p_index_b, real_t *adapted_values, const real_t *p_values, const real_t p_width, const real_t p_max_a, const real_t p_max_b) {
	if (p_values[p_index_a] + p_values[p_index_b] > p_width) {
		real_t factor;
		real_t new_value;

		factor = (real_t)p_width / (real_t)(p_values[p_index_a] + p_values[p_index_b]);

		new_value = (p_values[p_index_a] * factor);
		if (new_value < adapted_values[p_index_a]) {
			adapted_values[p_index_a] = new_value;
		}
		new_value = (p_values[p_index_b] * factor);
		if (new_value < adapted_values[p_index_b]) {
			adapted_values[p_index_b] = new_value;
		}
	} else {
		adapted_values[p_index_a] = MIN(p_values[p_index_a], adapted_values[p_index_a]);
		adapted_values[p_index_b] = MIN(p_values[p_index_b], adapted_values[p_index_b]);
	}
	adapted_values[p_index_a] = MIN(p_max_a, adapted_values[p_index_a]);
	adapted_values[p_index_b] = MIN(p_max_b, adapted_values[p_index_b]);
}

Rect2 StyleBoxFlat::get_draw_rect(const Rect2 &p_rect) const {
	Rect2 draw_rect = p_rect.grow_individual(expand_margin[SIDE_LEFT], expand_margin[SIDE_TOP], expand_margin[SIDE_RIGHT], expand_margin[SIDE_BOTTOM]);

	if (shadow_size > 0) {
		Rect2 shadow_rect = draw_rect.grow(shadow_size);
		shadow_rect.position += shadow_offset;
		draw_rect = draw_rect.merge(shadow_rect);
	}

	return draw_rect;
}

void StyleBoxFlat::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	bool draw_border = (border_width[0] > 0) || (border_width[1] > 0) || (border_width[2] > 0) || (border_width[3] > 0);
	bool draw_shadow = (shadow_size > 0);
	if (!draw_border && !draw_center && !draw_shadow) {
		return;
	}

	Rect2 style_rect = p_rect.grow_individual(expand_margin[SIDE_LEFT], expand_margin[SIDE_TOP], expand_margin[SIDE_RIGHT], expand_margin[SIDE_BOTTOM]);
	if (Math::is_zero_approx(style_rect.size.width) || Math::is_zero_approx(style_rect.size.height)) {
		return;
	}

	const bool rounded_corners = (corner_radius[0] > 0) || (corner_radius[1] > 0) || (corner_radius[2] > 0) || (corner_radius[3] > 0);
	// Only enable antialiasing if it is actually needed. This improve performances
	// and maximizes sharpness for non-skewed StyleBoxes with sharp corners.
	const bool aa_on = (rounded_corners || !skew.is_zero_approx()) && anti_aliased;

	const bool blend_on = blend_border && draw_border;

	Color border_color_alpha = Color(border_color.r, border_color.g, border_color.b, 0);
	Color border_color_blend = (draw_center ? bg_color : border_color_alpha);
	Color border_color_inner = blend_on ? border_color_blend : border_color;

	// Adapt borders (prevent weird overlapping/glitchy drawings).
	real_t width = MAX(style_rect.size.width, 0);
	real_t height = MAX(style_rect.size.height, 0);
	real_t adapted_border[4] = { 1000000.0, 1000000.0, 1000000.0, 1000000.0 };
	adapt_values(SIDE_TOP, SIDE_BOTTOM, adapted_border, border_width, height, height, height);
	adapt_values(SIDE_LEFT, SIDE_RIGHT, adapted_border, border_width, width, width, width);

	// Adapt corners (prevent weird overlapping/glitchy drawings).
	real_t adapted_corner[4] = { 1000000.0, 1000000.0, 1000000.0, 1000000.0 };
	adapt_values(CORNER_TOP_RIGHT, CORNER_BOTTOM_RIGHT, adapted_corner, corner_radius, height, height - adapted_border[SIDE_BOTTOM], height - adapted_border[SIDE_TOP]);
	adapt_values(CORNER_TOP_LEFT, CORNER_BOTTOM_LEFT, adapted_corner, corner_radius, height, height - adapted_border[SIDE_BOTTOM], height - adapted_border[SIDE_TOP]);
	adapt_values(CORNER_TOP_LEFT, CORNER_TOP_RIGHT, adapted_corner, corner_radius, width, width - adapted_border[SIDE_RIGHT], width - adapted_border[SIDE_LEFT]);
	adapt_values(CORNER_BOTTOM_LEFT, CORNER_BOTTOM_RIGHT, adapted_corner, corner_radius, width, width - adapted_border[SIDE_RIGHT], width - adapted_border[SIDE_LEFT]);

	Rect2 infill_rect = style_rect.grow_individual(-adapted_border[SIDE_LEFT], -adapted_border[SIDE_TOP], -adapted_border[SIDE_RIGHT], -adapted_border[SIDE_BOTTOM]);

	Rect2 border_style_rect = style_rect;
	if (aa_on) {
		for (int i = 0; i < 4; i++) {
			if (border_width[i] > 0) {
				border_style_rect = border_style_rect.grow_side((Side)i, -aa_size);
			}
		}
	}

	Vector<Point2> verts;
	Vector<int> indices;
	Vector<Color> colors;
	Vector<Point2> uvs;

	// Create shadow
	if (draw_shadow) {
		Rect2 shadow_inner_rect = style_rect;
		shadow_inner_rect.position += shadow_offset;

		Rect2 shadow_rect = style_rect.grow(shadow_size);
		shadow_rect.position += shadow_offset;

		Color shadow_color_transparent = Color(shadow_color.r, shadow_color.g, shadow_color.b, 0);

		draw_ring(verts, indices, colors, shadow_inner_rect, adapted_corner,
				shadow_rect, shadow_inner_rect, shadow_color, shadow_color_transparent, corner_detail, skew);

		if (draw_center) {
			draw_ring(verts, indices, colors, shadow_inner_rect, adapted_corner,
					shadow_inner_rect, shadow_inner_rect, shadow_color, shadow_color, corner_detail, skew, true);
		}
	}

	// Create border (no AA).
	if (draw_border && !aa_on) {
		draw_ring(verts, indices, colors, border_style_rect, adapted_corner,
				border_style_rect, infill_rect, border_color_inner, border_color, corner_detail, skew);
	}

	// Create infill (no AA).
	if (draw_center && (!aa_on || blend_on || !draw_border)) {
		draw_ring(verts, indices, colors, border_style_rect, adapted_corner,
				infill_rect, infill_rect, bg_color, bg_color, corner_detail, skew, true);
	}

	if (aa_on) {
		real_t aa_border_width[4];
		real_t aa_fill_width[4];
		if (draw_border) {
			for (int i = 0; i < 4; i++) {
				if (border_width[i] > 0) {
					aa_border_width[i] = aa_size;
					aa_fill_width[i] = 0;
				} else {
					aa_border_width[i] = 0;
					aa_fill_width[i] = aa_size;
				}
			}
		} else {
			for (int i = 0; i < 4; i++) {
				aa_border_width[i] = 0;
				aa_fill_width[i] = aa_size;
			}
		}

		Rect2 infill_inner_rect = infill_rect.grow_individual(-aa_border_width[SIDE_LEFT], -aa_border_width[SIDE_TOP],
				-aa_border_width[SIDE_RIGHT], -aa_border_width[SIDE_BOTTOM]);

		if (draw_center) {
			if (!blend_on && draw_border) {
				Rect2 infill_inner_rect_aa = infill_inner_rect.grow_individual(aa_border_width[SIDE_LEFT], aa_border_width[SIDE_TOP],
						aa_border_width[SIDE_RIGHT], aa_border_width[SIDE_BOTTOM]);
				// Create infill within AA border.
				draw_ring(verts, indices, colors, border_style_rect, adapted_corner,
						infill_inner_rect_aa, infill_inner_rect_aa, bg_color, bg_color, corner_detail, skew, true);
			}

			if (!blend_on || !draw_border) {
				Rect2 infill_rect_aa = infill_rect.grow_individual(aa_fill_width[SIDE_LEFT], aa_fill_width[SIDE_TOP],
						aa_fill_width[SIDE_RIGHT], aa_fill_width[SIDE_BOTTOM]);

				Color alpha_bg = Color(bg_color.r, bg_color.g, bg_color.b, 0);

				// Create infill fake AA gradient.
				draw_ring(verts, indices, colors, style_rect, adapted_corner,
						infill_rect_aa, infill_rect, bg_color, alpha_bg, corner_detail, skew);
			}
		}

		if (draw_border) {
			Rect2 infill_rect_aa = infill_rect.grow_individual(aa_border_width[SIDE_LEFT], aa_border_width[SIDE_TOP],
					aa_border_width[SIDE_RIGHT], aa_border_width[SIDE_BOTTOM]);
			Rect2 style_rect_aa = style_rect.grow_individual(aa_border_width[SIDE_LEFT], aa_border_width[SIDE_TOP],
					aa_border_width[SIDE_RIGHT], aa_border_width[SIDE_BOTTOM]);
			Rect2 border_style_rect_aa = border_style_rect.grow_individual(aa_border_width[SIDE_LEFT], aa_border_width[SIDE_TOP],
					aa_border_width[SIDE_RIGHT], aa_border_width[SIDE_BOTTOM]);

			// Create border.
			draw_ring(verts, indices, colors, border_style_rect, adapted_corner,
					border_style_rect_aa, ((blend_on) ? infill_rect : infill_rect_aa), border_color_inner, border_color, corner_detail, skew);

			if (!blend_on) {
				// Create inner border fake AA gradient.
				draw_ring(verts, indices, colors, border_style_rect, adapted_corner,
						infill_rect_aa, infill_rect, border_color_blend, border_color, corner_detail, skew);
			}

			// Create outer border fake AA gradient.
			draw_ring(verts, indices, colors, border_style_rect, adapted_corner,
					style_rect_aa, border_style_rect_aa, border_color, border_color_alpha, corner_detail, skew);
		}
	}

	// Compute UV coordinates.
	Rect2 uv_rect = style_rect.grow(aa_on ? aa_size : 0);
	uvs.resize(verts.size());
	for (int i = 0; i < verts.size(); i++) {
		uvs.write[i].x = (verts[i].x - uv_rect.position.x) / uv_rect.size.width;
		uvs.write[i].y = (verts[i].y - uv_rect.position.y) / uv_rect.size.height;
	}

	// Draw stylebox.
	RenderingServer *vs = RenderingServer::get_singleton();
	vs->canvas_item_add_triangle_array(p_canvas_item, indices, verts, colors, uvs);
}

float StyleBoxFlat::get_style_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);
	return border_width[p_side];
}

void StyleBoxFlat::_validate_property(PropertyInfo &p_property) const {
	if (!anti_aliased && p_property.name == "anti_aliasing_size") {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}

void StyleBoxFlat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_bg_color", "color"), &StyleBoxFlat::set_bg_color);
	ClassDB::bind_method(D_METHOD("get_bg_color"), &StyleBoxFlat::get_bg_color);

	ClassDB::bind_method(D_METHOD("set_border_color", "color"), &StyleBoxFlat::set_border_color);
	ClassDB::bind_method(D_METHOD("get_border_color"), &StyleBoxFlat::get_border_color);

	ClassDB::bind_method(D_METHOD("set_border_width_all", "width"), &StyleBoxFlat::set_border_width_all);
	ClassDB::bind_method(D_METHOD("get_border_width_min"), &StyleBoxFlat::get_border_width_min);

	ClassDB::bind_method(D_METHOD("set_border_width", "margin", "width"), &StyleBoxFlat::set_border_width);
	ClassDB::bind_method(D_METHOD("get_border_width", "margin"), &StyleBoxFlat::get_border_width);

	ClassDB::bind_method(D_METHOD("set_border_blend", "blend"), &StyleBoxFlat::set_border_blend);
	ClassDB::bind_method(D_METHOD("get_border_blend"), &StyleBoxFlat::get_border_blend);

	ClassDB::bind_method(D_METHOD("set_corner_radius_individual", "radius_top_left", "radius_top_right", "radius_bottom_right", "radius_bottom_left"), &StyleBoxFlat::set_corner_radius_individual);
	ClassDB::bind_method(D_METHOD("set_corner_radius_all", "radius"), &StyleBoxFlat::set_corner_radius_all);

	ClassDB::bind_method(D_METHOD("set_corner_radius", "corner", "radius"), &StyleBoxFlat::set_corner_radius);
	ClassDB::bind_method(D_METHOD("get_corner_radius", "corner"), &StyleBoxFlat::get_corner_radius);

	ClassDB::bind_method(D_METHOD("set_expand_margin", "margin", "size"), &StyleBoxFlat::set_expand_margin_size);
	ClassDB::bind_method(D_METHOD("set_expand_margin_all", "size"), &StyleBoxFlat::set_expand_margin_size_all);
	ClassDB::bind_method(D_METHOD("set_expand_margin_individual", "size_left", "size_top", "size_right", "size_bottom"), &StyleBoxFlat::set_expand_margin_size_individual);
	ClassDB::bind_method(D_METHOD("get_expand_margin", "margin"), &StyleBoxFlat::get_expand_margin_size);

	ClassDB::bind_method(D_METHOD("set_draw_center", "draw_center"), &StyleBoxFlat::set_draw_center);
	ClassDB::bind_method(D_METHOD("is_draw_center_enabled"), &StyleBoxFlat::is_draw_center_enabled);

	ClassDB::bind_method(D_METHOD("set_skew", "skew"), &StyleBoxFlat::set_skew);
	ClassDB::bind_method(D_METHOD("get_skew"), &StyleBoxFlat::get_skew);

	ClassDB::bind_method(D_METHOD("set_shadow_color", "color"), &StyleBoxFlat::set_shadow_color);
	ClassDB::bind_method(D_METHOD("get_shadow_color"), &StyleBoxFlat::get_shadow_color);

	ClassDB::bind_method(D_METHOD("set_shadow_size", "size"), &StyleBoxFlat::set_shadow_size);
	ClassDB::bind_method(D_METHOD("get_shadow_size"), &StyleBoxFlat::get_shadow_size);

	ClassDB::bind_method(D_METHOD("set_shadow_offset", "offset"), &StyleBoxFlat::set_shadow_offset);
	ClassDB::bind_method(D_METHOD("get_shadow_offset"), &StyleBoxFlat::get_shadow_offset);

	ClassDB::bind_method(D_METHOD("set_anti_aliased", "anti_aliased"), &StyleBoxFlat::set_anti_aliased);
	ClassDB::bind_method(D_METHOD("is_anti_aliased"), &StyleBoxFlat::is_anti_aliased);

	ClassDB::bind_method(D_METHOD("set_aa_size", "size"), &StyleBoxFlat::set_aa_size);
	ClassDB::bind_method(D_METHOD("get_aa_size"), &StyleBoxFlat::get_aa_size);

	ClassDB::bind_method(D_METHOD("set_corner_detail", "detail"), &StyleBoxFlat::set_corner_detail);
	ClassDB::bind_method(D_METHOD("get_corner_detail"), &StyleBoxFlat::get_corner_detail);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "bg_color"), "set_bg_color", "get_bg_color");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_center"), "set_draw_center", "is_draw_center_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "skew"), "set_skew", "get_skew");

	ADD_GROUP("Border Width", "border_width_");
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_left", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_border_width", "get_border_width", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_top", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_border_width", "get_border_width", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_right", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_border_width", "get_border_width", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "border_width_bottom", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_border_width", "get_border_width", SIDE_BOTTOM);

	ADD_GROUP("Border", "border_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "border_color"), "set_border_color", "get_border_color");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "border_blend"), "set_border_blend", "get_border_blend");

	ADD_GROUP("Corner Radius", "corner_radius_");
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_top_left", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_TOP_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_top_right", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_TOP_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_bottom_right", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_BOTTOM_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "corner_radius_bottom_left", PROPERTY_HINT_RANGE, "0,1024,1,suffix:px"), "set_corner_radius", "get_corner_radius", CORNER_BOTTOM_LEFT);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "corner_detail", PROPERTY_HINT_RANGE, "1,20,1"), "set_corner_detail", "get_corner_detail");

	ADD_GROUP("Expand Margins", "expand_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_left", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_top", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_right", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "expand_margin_bottom", PROPERTY_HINT_RANGE, "0,2048,1,suffix:px"), "set_expand_margin", "get_expand_margin", SIDE_BOTTOM);

	ADD_GROUP("Shadow", "shadow_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "shadow_color"), "set_shadow_color", "get_shadow_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shadow_size", PROPERTY_HINT_RANGE, "0,100,1,or_greater,suffix:px"), "set_shadow_size", "get_shadow_size");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shadow_offset", PROPERTY_HINT_NONE, "suffix:px"), "set_shadow_offset", "get_shadow_offset");

	ADD_GROUP("Anti Aliasing", "anti_aliasing_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "anti_aliasing"), "set_anti_aliased", "is_anti_aliased");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "anti_aliasing_size", PROPERTY_HINT_RANGE, "0.01,10,0.001,suffix:px"), "set_aa_size", "get_aa_size");
}

StyleBoxFlat::StyleBoxFlat() {}

StyleBoxFlat::~StyleBoxFlat() {}

void StyleBoxLine::set_color(const Color &p_color) {
	color = p_color;
	emit_changed();
}

Color StyleBoxLine::get_color() const {
	return color;
}

void StyleBoxLine::set_thickness(int p_thickness) {
	thickness = p_thickness;
	emit_changed();
}

int StyleBoxLine::get_thickness() const {
	return thickness;
}

void StyleBoxLine::set_vertical(bool p_vertical) {
	vertical = p_vertical;
	emit_changed();
}

bool StyleBoxLine::is_vertical() const {
	return vertical;
}

void StyleBoxLine::set_grow_end(float p_grow_end) {
	grow_end = p_grow_end;
	emit_changed();
}

float StyleBoxLine::get_grow_end() const {
	return grow_end;
}

void StyleBoxLine::set_grow_begin(float p_grow_begin) {
	grow_begin = p_grow_begin;
	emit_changed();
}

float StyleBoxLine::get_grow_begin() const {
	return grow_begin;
}

void StyleBoxLine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_color", "color"), &StyleBoxLine::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &StyleBoxLine::get_color);
	ClassDB::bind_method(D_METHOD("set_thickness", "thickness"), &StyleBoxLine::set_thickness);
	ClassDB::bind_method(D_METHOD("get_thickness"), &StyleBoxLine::get_thickness);
	ClassDB::bind_method(D_METHOD("set_grow_begin", "offset"), &StyleBoxLine::set_grow_begin);
	ClassDB::bind_method(D_METHOD("get_grow_begin"), &StyleBoxLine::get_grow_begin);
	ClassDB::bind_method(D_METHOD("set_grow_end", "offset"), &StyleBoxLine::set_grow_end);
	ClassDB::bind_method(D_METHOD("get_grow_end"), &StyleBoxLine::get_grow_end);
	ClassDB::bind_method(D_METHOD("set_vertical", "vertical"), &StyleBoxLine::set_vertical);
	ClassDB::bind_method(D_METHOD("is_vertical"), &StyleBoxLine::is_vertical);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "grow_begin", PROPERTY_HINT_RANGE, "-300,300,1,suffix:px"), "set_grow_begin", "get_grow_begin");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "grow_end", PROPERTY_HINT_RANGE, "-300,300,1,suffix:px"), "set_grow_end", "get_grow_end");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "thickness", PROPERTY_HINT_RANGE, "0,10,suffix:px"), "set_thickness", "get_thickness");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vertical"), "set_vertical", "is_vertical");
}

float StyleBoxLine::get_style_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);

	if (vertical) {
		if (p_side == SIDE_LEFT || p_side == SIDE_RIGHT) {
			return thickness / 2.0;
		}
	} else if (p_side == SIDE_TOP || p_side == SIDE_BOTTOM) {
		return thickness / 2.0;
	}

	return 0;
}

Size2 StyleBoxLine::get_center_size() const {
	return Size2();
}

void StyleBoxLine::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	RenderingServer *vs = RenderingServer::get_singleton();
	Rect2i r = p_rect;

	if (vertical) {
		r.position.y -= grow_begin;
		r.size.y += (grow_begin + grow_end);
		r.size.x = thickness;
	} else {
		r.position.x -= grow_begin;
		r.size.x += (grow_begin + grow_end);
		r.size.y = thickness;
	}

	vs->canvas_item_add_rect(p_canvas_item, r, color);
}

StyleBoxLine::StyleBoxLine() {}

StyleBoxLine::~StyleBoxLine() {}
