package main

import (
	"fmt"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/data/binding"
	"fyne.io/fyne/v2/widget"
)

// CustomList extends widget.List to support custom key handling
type LofiList struct {
	widget.List

	OnKeyDown func(key *fyne.KeyEvent)
}

func NewLofiList(length func() int, createItem func() fyne.CanvasObject, updateItem func(widget.ListItemID, fyne.CanvasObject)) *LofiList {
	list := &LofiList{}
	list.Length = length
	list.CreateItem = createItem
	list.UpdateItem = updateItem
	list.ExtendBaseWidget(list)
	return list
}

func NewLofiListWithData(data binding.DataList, createItem func() fyne.CanvasObject, updateItem func(binding.DataItem, fyne.CanvasObject)) *LofiList {
	list := NewLofiList(
		data.Length,
		createItem,
		func(i widget.ListItemID, o fyne.CanvasObject) {
			item, err := data.GetItem(i)
			if err != nil {
				fyne.LogError(fmt.Sprintf("Error getting data item %d", i), err)
				return
			}
			updateItem(item, o)
		})

	data.AddListener(binding.NewDataListener(list.Refresh))
	return list
}

func (c *LofiList) TypedKey(key *fyne.KeyEvent) {
	if c.OnKeyDown != nil {
		c.OnKeyDown(key)
	}
	c.List.TypedKey(key) // Ensure the default behavior is still provided
}
