package main

import (
	"log"
	"strconv"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/driver/desktop"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
)

func main() {
	a := app.New()

	// Create the main window as a splash window
	drv := fyne.CurrentApp().Driver()
	var w fyne.Window
	if desktopDrv, ok := drv.(desktop.Driver); ok {
		w = desktopDrv.CreateSplashWindow() // Create the main window as a splash window
	}

	// Set the window size and position
	w.Resize(fyne.NewSize(600, 400))
	w.CenterOnScreen()

	textBox := widget.NewEntry()
	textBox.SetPlaceHolder("Type here...")
	labelList := container.NewVBox()

	for i := 1; i <= 10; i++ {
		label := widget.NewLabel("Item " + strconv.Itoa(i)) // Corrected conversion
		labelList.Add(label)
	}

	w.SetContent(container.NewVBox(
		textBox,
		labelList,
	))

	// Handling Escape to hide the window
	if deskCanvas, ok := w.Canvas().(desktop.Canvas); ok {
		deskCanvas.SetOnKeyDown(func(key *fyne.KeyEvent) {
			log.Println("Escape pressed, hiding window")
			w.Hide()
		})
	}

	// Handling Ctrl+D to kill the app
	w.Canvas().AddShortcut(&desktop.CustomShortcut{
		KeyName:  fyne.KeyD,
		Modifier: fyne.KeyModifierControl,
	}, func(shortcut fyne.Shortcut) {
		log.Println("Ctrl+D pressed, quitting app")
		a.Quit()
	})

	// Handling Alt+Space to show the window and reset its content
	// TODO: Does not work if window is not selected
	w.Canvas().AddShortcut(&desktop.CustomShortcut{
		KeyName:  fyne.KeySpace,
		Modifier: fyne.KeyModifierAlt,
	}, func(shortcut fyne.Shortcut) {
		log.Println("Alt+Space pressed, showing window and resetting content")
		textBox.SetText("")
		labelList.Objects = []fyne.CanvasObject{}
		for i := 1; i <= 10; i++ {
			label := widget.NewLabel("Item " + strconv.Itoa(i)) // Reset content
			labelList.Add(label)
		}
		w.Show()
	})

	// Set system tray menu and icon
	if desk, ok := a.(desktop.App); ok {
		trayIcon := theme.FyneLogo() // You can replace this with your own icon, e.g., fyne.NewStaticResource("icon.png", iconPNGBytes)
		desk.SetSystemTrayIcon(trayIcon)

		m := fyne.NewMenu("MyApp",
			fyne.NewMenuItem("Show", func() { w.Show() }),
			fyne.NewMenuItem("Quit", func() { a.Quit() }),
		)
		desk.SetSystemTrayMenu(m)
	}

	// Set intercept to hide the window instead of closing it
	w.SetCloseIntercept(func() {
		w.Hide()
	})

	// Show the main splash window
	w.Show()

	// Run the application
	fyne.CurrentApp().Driver().Run()
}
