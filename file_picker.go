package main

import (
	"errors"
	"io/fs"
	"log"
	"os/exec"
	"path/filepath"
	"strings"
)

func searchFiles(query string) {
	local_captures := []any{}
	i := 0
	err := filepath.WalkDir(".", func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		if !d.IsDir() && strings.Contains(strings.ToLower(d.Name()), strings.ToLower(query)) {
			local_captures = append(local_captures, Capture{id: i, showName: path})
			i++
		}
		return nil
	})
	if err != nil {
		log.Println("Error:", err)
	}

	captures.Set(local_captures)
}

func open(path string) error {
	switch {
	case exec.Command("rundll32", "url.dll,FileProtocolHandler", path).Start() == nil:
		return nil
	case exec.Command("xdg-open", path).Start() == nil:
		return nil
	case exec.Command("open", path).Start() == nil:
		return nil
	}
	return errors.New("unsupported platform")
}
