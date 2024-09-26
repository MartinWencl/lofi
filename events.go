package main

import (
	"log"

	"fyne.io/fyne/v2"
)

func handleEvents(w fyne.Window) {
	w.Canvas().SetOnTypedKey(func(key *fyne.KeyEvent) {
		switch key.Name {
		case fyne.KeyTab:
			log.Println("Sel. Up - captured at canvas event handler")
		}

	})

	textBoxEventHandlers(w)
	listEventHandlers(w)
}

func textBoxEventHandlers(w fyne.Window) {
	textBox.OnSubmitted = func(s string) {
		log.Println("Return - captured at textBox event handler")
		if currentMode.OnSubmitted == nil {
			log.Panic("Error function nil!")
		}
		currentMode.OnSubmitted(s)
		changeState(SELECTING, w)
	}
}

func listEventHandlers(w fyne.Window) {
	appList.OnKeyDown = func(key *fyne.KeyEvent) {
		switch key.Name {
		case fyne.KeyUp:
			selectItem(-1)
			log.Println("Sel. Up - captured at list event handler")
		case fyne.KeyK:
			appList.TypedKey(&fyne.KeyEvent{Name: fyne.KeyUp, Physical: fyne.HardwareKey{}})
		case fyne.KeyDown:
			selectItem(1)
			log.Println("Sel. Down - captured at list event handler")
		case fyne.KeyJ:
			appList.TypedKey(&fyne.KeyEvent{Name: fyne.KeyDown, Physical: fyne.HardwareKey{}})
		case fyne.KeyEnter, fyne.KeyReturn:
			if currentSelection >= 0 && currentSelection < captures.Length() {
				var capture, _ = getCapture(currentSelection)
				log.Println("Return - captured at list event handler - with capture", capture.showName, capture.id)
				currentMode.OnSelected(capture)
			}
		case fyne.KeyI, fyne.KeyTab:
			changeState(SEARCHING, w)
			log.Println("I/Tab - captured at list event handler - changing state")
		case fyne.KeyEscape:
			w.Hide()
		case fyne.KeyQ:
			w.Hide()
		}
	}
}
