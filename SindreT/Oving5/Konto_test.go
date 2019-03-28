package main

import (
	"fmt"
	"github.com/jinzhu/gorm"
	"testing"
)

func TestKonto(t *testing.T) {
	url := "sindrtho:joVjipuw@tcp(mysql.stud.iie.ntnu.no:3306)/sindrtho?charset=utf8&parseTime=True&loc=Local"
	db, err := gorm.Open("mysql", url)
	defer db.Close()
	if err != nil {
		fmt.Print(err)
		return
	}

	dbInit(db)

	kontos := []Konto {
		{1, "sindre", 100},
		{2, "jan", 1000},
		{3, "bob", 5000},
		{4, "kari", 10000},
		{5, "alfred", 250},
	}

	for _, e := range kontos {
		newKonto(db, e.Navn)
		db.Save(&e)
		printKonto(getKonto(db, e.Kontonummer))
	}


	fmt.Print("Tester at vi finner de rike. Alle som har over 3000kr")
	richPeople := whosRich(db, 3000)

	for _, e := range richPeople {
		//Kontoene er 3 og 4, jan og bob
		if e.Kontonummer != 3 && e.Kontonummer != 4 {
			t.Errorf("NEI!")
		}
		printKonto(e)
	}

	for _, e := range richPeople {
		e.Navn = "sindre"
		db.Save(&e)
	}

	fmt.Print("Ser hvem som er rik etter navnet ble byttet")
	richPeople = whosRich(db, 3000)

	for _, e := range richPeople {
		if e.Navn != "sindre" {
			t.Errorf("NEI!")
		}
		printKonto(e)
	}
}