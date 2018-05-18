#pragma once

#include "BrowserItem.h"
#include "OpenGL/Drawing.h"
#include "UI/Canvas/OGLCanvas.h"

class wxScrollBar;

class BrowserCanvas : public OGLCanvas
{
public:
	BrowserCanvas(wxWindow* parent);
	~BrowserCanvas() = default;

	vector<BrowserItem*>& itemList() { return items_; }
	int                   getViewedIndex();
	void                  addItem(BrowserItem* item);
	void                  clearItems();
	int                   fullItemSizeX() const;
	int                   fullItemSizeY() const;
	void                  draw() override;
	void                  setScrollBar(wxScrollBar* scrollbar);
	void                  updateLayout(int viewed_item = -1);
	BrowserItem*          selectedItem() const { return item_selected_; }
	BrowserItem*          itemAt(int index);
	int                   itemIndex(BrowserItem* item);
	void                  selectItem(int index);
	void                  selectItem(BrowserItem* item);
	void                  filterItems(string_view filter);
	void                  showItem(int item, int where);
	void                  showSelectedItem();
	bool                  searchItemFrom(int from);
	void                  setFont(Drawing::Font font) { font_ = font; }
	void                  setItemNameType(BrowserItem::NameType type) { show_names_ = type; }
	void                  setItemSize(int size) { item_size_ = size; }
	void                  setItemViewType(BrowserItem::ViewType type) { item_type_ = type; }
	int                   longestItemTextWidth() const;

	// Events
	void onSize(wxSizeEvent& e);
	void onScrollThumbTrack(wxScrollEvent& e);
	void onScrollLineUp(wxScrollEvent& e);
	void onScrollLineDown(wxScrollEvent& e);
	void onScrollPageUp(wxScrollEvent& e);
	void onScrollPageDown(wxScrollEvent& e);
	void onMouseEvent(wxMouseEvent& e);
	void onKeyDown(wxKeyEvent& e);
	void onKeyChar(wxKeyEvent& e);

private:
	vector<BrowserItem*> items_;
	vector<int>          items_filter_;
	wxScrollBar*         scrollbar_ = nullptr;
	string               search_;
	BrowserItem*         item_selected_ = nullptr;

	// Display
	int                   yoff_        = 0;
	int                   item_border_ = 0;
	Drawing::Font         font_        = Drawing::Font::Bold;
	BrowserItem::NameType show_names_  = BrowserItem::NameType::Normal;
	int                   item_size_   = -1;
	int                   top_index_   = 0;
	int                   top_y_       = 0;
	BrowserItem::ViewType item_type_   = BrowserItem::ViewType::Normal;
	int                   num_cols_    = -1;
};

DECLARE_EVENT_TYPE(wxEVT_BROWSERCANVAS_SELECTION_CHANGED, -1)
