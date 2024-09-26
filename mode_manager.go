package main

import (
	"log"

	"fyne.io/fyne/v2"
)

type Mode struct {
	Name         string
	Description  string
	OnActivate   func(fyne.Window)
	OnDeactivate func(fyne.Window)
	OnSubmitted  func(string)
	OnSelected   func(Capture)
}

var modes = make(map[string]*Mode)
var currentMode *Mode

func RegisterMode(name string, onActivate, onDeactivate func(fyne.Window), OnSelected func(Capture), OnSubmitted func(string)) {
	modes[name] = &Mode{
		Name:         name,
		OnActivate:   onActivate,
		OnDeactivate: onDeactivate,
		OnSubmitted:  OnSubmitted,
		OnSelected:   OnSelected,
	}
}

func SetCurrentMode(name string, w fyne.Window) {
	if mode, exists := modes[name]; exists {
		if currentMode != nil && currentMode.OnDeactivate != nil {
			currentMode.OnDeactivate(w)
		}
		currentMode = mode
		if currentMode.OnActivate != nil {
			currentMode.OnActivate(w)
		}

		log.Println("Changed mode to", mode)
	}
}

func RegisterDefaultModes(w fyne.Window) {
	RegisterMode(
		"filePicker",
		func(w fyne.Window) {
			changeState(SEARCHING, w)
		},
		nil,
		func(c Capture) {
			open(c.showName)
		},
		func(s string) {
			searchFiles(s)
		})
	SetCurrentMode("filePicker", w)
}
