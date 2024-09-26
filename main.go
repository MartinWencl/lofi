package main

import (
	"log"

	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/data/binding"
)

func main() {
	a := app.NewWithID("com.example.roficlone")

	captures = binding.NewUntypedList()

	w := createMainWindow(a)
	setupSystemTray(a, w)

	RegisterDefaultModes(w)
	handleEvents(w)

	test()

	w.Show()
	w.Canvas().Focus(textBox)
	log.Println("Application started")
	a.Run()
}
