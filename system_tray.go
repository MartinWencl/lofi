package main

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/driver/desktop"
	"fyne.io/fyne/v2/theme"
)

func setupSystemTray(a fyne.App, w fyne.Window) {
	if desk, ok := a.(desktop.App); ok {
		trayIcon := theme.FyneLogo()
		desk.SetSystemTrayIcon(trayIcon)

		m := fyne.NewMenu("App",
			fyne.NewMenuItem("Show", func() { w.Show() }),
			fyne.NewMenuItem("Quit", func() { a.Quit() }),
		)
		desk.SetSystemTrayMenu(m)
	}
}
