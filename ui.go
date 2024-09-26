package main

import (
	"log"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/data/binding"
	"fyne.io/fyne/v2/driver/desktop"
	"fyne.io/fyne/v2/widget"
)

var textBox *widget.Entry
var appList *LofiList

func createMainWindow(a fyne.App) fyne.Window {
	appSize := fyne.NewSize(600, 400)
	const c_offset float32 = 0.4

	drv := fyne.CurrentApp().Driver()
	var w fyne.Window
	if desktopDrv, ok := drv.(desktop.Driver); ok {
		w = desktopDrv.CreateSplashWindow()
	}

	w.Resize(appSize)
	w.CenterOnScreen()

	textBox = widget.NewEntry()
	textBox.SetPlaceHolder("Type here...")

	appList = NewLofiListWithData(
		captures,
		func() fyne.CanvasObject {
			var button = widget.NewButton("Button", func() {})
			button.Importance = widget.LowImportance
			return button
		},
		func(item binding.DataItem, obj fyne.CanvasObject) {
			button := obj.(*widget.Button)
			val, err := item.(binding.Untyped).Get()
			if err != nil {
				log.Println("Failed to get item:", err)
				return
			}
			capture, ok := val.(Capture)
			if !ok {
				log.Println("Type assertion failed")
				return
			}
			button.Text = capture.showName
			button.OnTapped = func() { currentMode.OnSelected(capture) }
			button.Refresh()
		},
	)

	content := container.NewVSplit(textBox, appList)
	textBox.Resize(fyne.NewSize(appSize.Width, appSize.Height*c_offset))
	textBox.TextStyle.Bold = true
	content.SetOffset(0.0)
	w.SetContent(content)

	return w
}
