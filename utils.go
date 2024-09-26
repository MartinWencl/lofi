package main

import (
	"errors"
	"log"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/data/binding"
)

type Capture struct {
	id       int
	showName string
}

var captures binding.UntypedList
var currentSelection = -1

// Define State type and its constants
type State int

const (
	SEARCHING State = iota
	SELECTING
)

var currentState State

func changeState(state State, w fyne.Window) {
	currentState = state
	switch state {
	case SEARCHING:
		w.Canvas().Focus(textBox)
	case SELECTING:
		if w.Canvas() == nil {
			log.Println("error canvas nil")
		}

		if currentSelection == -1 && captures.Length() > 0 {
			currentSelection = 0
		}
		log.Println(currentSelection)
		w.Canvas().Focus(appList)
	}
}

func selectItem(direction int) {
	if captures.Length() == 0 {
		return
	}
	previousSelection := currentSelection
	currentSelection += direction
	if currentSelection < 0 {
		currentSelection = 0
	} else if currentSelection >= captures.Length() {
		currentSelection = captures.Length() - 1
	}
	if previousSelection >= 0 && previousSelection < captures.Length() {
		appList.RefreshItem(previousSelection)
	}
	appList.RefreshItem(currentSelection)
}

func getCapture(id int) (c Capture, e error) {
	item, err := captures.GetItem(id)
	if err != nil || item == nil {
		return Capture{}, errors.New("failed to get capture by id")
	}
	untypedItem := item.(binding.Untyped)
	val, err := untypedItem.Get()
	if err != nil {
		return Capture{}, errors.New("failed getting capture from untyped")
	}
	capture, ok := val.(Capture)
	if !ok {
		return Capture{}, errors.New("failed to assert type capture")
	}
	return capture, nil
}
