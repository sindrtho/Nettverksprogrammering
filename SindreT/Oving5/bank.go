package main

import (
	"bufio"
	"fmt"
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/mysql"
	"os"
	"strconv"
	"strings"
)

type Konto struct {
	//gorm.Model
	Kontonummer int `gorm:"primary_key";"AUTO_INCREMENT"`
	Navn string		`gorm:"type:varchar(32)"`
	Saldo float64 `gorm:"default:0"`
}

func main() {
	helpMenu :=
		"new:		Lager en ny konto\n" +
		"deposit:	Sett inn penger\n" +
		"withdraw: 	Ta ut penger\n" +
		"transfer: 	Overfør penger\n" +
		"owner:		Skift eier av konto\n\n"

	url := "sindrtho:joVjipuw@tcp(mysql.stud.iie.ntnu.no:3306)/sindrtho?charset=utf8&parseTime=True&loc=Local"

	db, err := gorm.Open("mysql", url)
	defer db.Close()
	if err != nil {
		fmt.Print(err)
		return
	}

	dbInit(db)

	reader := bufio.NewReader(os.Stdin)

	fmt.Print(helpMenu)
	myString, err := reader.ReadString('\n')
	if err != nil {
		fmt.Print("Something went wrong", err)
	}

	for myString != "exit\n" {
		switch myString {
		case "new\n":
			fmt.Print("Navn på kontoeier: ")
			name, _ := reader.ReadString('\n')
			newKonto(db, strings.Trim(name, "\n"))
		case "owner\n":
			updateOwner(db, reader)
		case "deposit\n":
			deposit(db, reader)
		case "withdraw\n":
			withdraw(db, reader)
		case "transfer\n":
			transfer(db, reader)
		case "check\n":
			checkKonto(db, reader)
		default:
			fmt.Print("You did something wrong\n\n")
		}
		myString, _ = reader.ReadString('\n')
	}
}

func checkKonto(db *gorm.DB, reader *bufio.Reader) {
	aRead, _ := reader.ReadString('\n')
	a, err := strconv.Atoi(strings.Trim(aRead, "\n"))
	for err != nil {
		aRead, _ = reader.ReadString('\n')
		a, err = strconv.Atoi(strings.Trim(aRead, "\n"))
	}
	konto := getKonto(db, a)
	printKonto(konto)
}

func newKonto(db *gorm.DB, navn string) {
	Konto := Konto{Navn: navn}

	if db.NewRecord(&Konto) {
		db.Create(&Konto)
		fmt.Print("Konto opprettet\n")
		printKonto(Konto)
	}
}

func getKonto(db *gorm.DB, kontonummer int) Konto {
	konto := Konto{}
	db.First(&konto, kontonummer)
	return konto
}

func withdraw(db *gorm.DB, reader *bufio.Reader) {
	beginFra:
	fmt.Print("Fra konto: ")
	fraRead, _ := reader.ReadString('\n')
	fra, err := strconv.Atoi(strings.Trim(fraRead, "\n"))
	for err != nil {
		if fraRead == "exit\n" {
			return
		}
		fmt.Print("Vennligst oppgi gyldig kontonummer.\nFra konto: ")
		fraRead, _ = reader.ReadString('\n')
		fra, err = strconv.Atoi(strings.Trim(fraRead, "\n"))
	}

	fraKonto := getKonto(db, fra)
	if fraKonto.Kontonummer != fra {
		fmt.Print("Kontoen finnes ikke. Prøv igjen\n\n")
		goto beginFra
	}

	fmt.Print("Beløp: ")
	belopRead, _ := reader.ReadString('\n')
	belop, err := strconv.ParseFloat(strings.Trim(belopRead, "\n"), 64)
	for err != nil {
		fmt.Print("Vennligst oppgi gyldig beløp.\nBelop: ")
		belopRead, _ = reader.ReadString('\n')
		belop, err = strconv.ParseFloat(strings.Trim(belopRead, "\n"), 64)
	}

	if fraKonto.Saldo >= belop {
		fraKonto.Saldo -= belop
		db.Save(&fraKonto)
		//fmt.Print("Ny saldo: ", fraKonto.Saldo)
		printKonto(fraKonto)
	} else {
		fmt.Print("Det er ikke nok penger på kontoen til å utføre transaksjonen.\n\n")
	}
}

func deposit(db *gorm.DB, reader *bufio.Reader) {
	beginTil:
	fmt.Print("Til konto: ")
	tilRead, _ := reader.ReadString('\n')
	til, err := strconv.Atoi(strings.Trim(tilRead, "\n"))
	for err != nil {
		if tilRead == "exit\n" {
			return
		}
		fmt.Print("Vennligst oppgi gyldig kontonummer.\nTil konto: ")
		tilRead, _ = reader.ReadString('\n')
		til, err = strconv.Atoi(strings.Trim(tilRead, "\n"))
	}
	tilKonto := getKonto(db, til)
	if tilKonto.Kontonummer != til {
		fmt.Print("Kontoen finnes ikke. Prøv igjen\n\n")
		goto beginTil
	}

	fmt.Print("Beløp: ")
	belopRead, _ := reader.ReadString('\n')
	belop, err := strconv.ParseFloat(strings.Trim(belopRead, "\n"), 64)
	for err != nil {
		fmt.Print("Vennligst oppgi gyldig beløp.\nBelop: ")
		belopRead, _ = reader.ReadString('\n')
		belop, err = strconv.ParseFloat(strings.Trim(belopRead, "\n"), 64)
	}

	tilKonto.Saldo += belop
	db.Save(tilKonto)

	//fmt.Print("Ny saldo: ", tilKonto.Saldo, "\n\n")
	printKonto(tilKonto)
}

func transfer(db *gorm.DB, reader *bufio.Reader) {
	beginFra:
	fmt.Print("Fra konto: ")
	fraRead, _ := reader.ReadString('\n')
	fra, err := strconv.Atoi(strings.Trim(fraRead, "\n"))
	for err != nil {
		if fraRead == "exit\n" {
			return
		}
		fmt.Print("Vennligst oppgi gyldig kontonummer.\nFra konto: ")
		fraRead, _ = reader.ReadString('\n')
		fra, err = strconv.Atoi(strings.Trim(fraRead, "\n"))
	}

	fraKonto := getKonto(db, fra)
	if fraKonto.Kontonummer != fra {
		fmt.Print("Kontoen finnes ikke. Prøv igjen\n\n")
		goto beginFra
	}

	beginTil:
	fmt.Print("Til konto: ")
	tilRead, _ := reader.ReadString('\n')
	til, err := strconv.Atoi(strings.Trim(tilRead, "\n"))
	for err != nil {
		if tilRead == "exit\n" {
			return
		}
		fmt.Print("Vennligst oppgi gyldig kontonummer.\nTil konto: ")
		tilRead, _ = reader.ReadString('\n')
		til, err = strconv.Atoi(strings.Trim(tilRead, "\n"))
	}
	tilKonto := getKonto(db, til)
	if tilKonto.Kontonummer != til {
		fmt.Print("Kontoen finnes ikke. Prøv igjen\n\n")
		goto beginTil
	}

	fmt.Print("Beløp: ")
	belopRead, _ := reader.ReadString('\n')
	belop, err := strconv.ParseFloat(strings.Trim(belopRead, "\n"), 64)
	for err != nil {
		fmt.Print("Vennligst oppgi gyldig beløp.\nBelop: ")
		belopRead, _ = reader.ReadString('\n')
		belop, err = strconv.ParseFloat(strings.Trim(belopRead, "\n"), 64)
	}

	if fraKonto.Saldo >= belop {
		fraKonto.Saldo -= belop
		tilKonto.Saldo += belop
		db.Save(&fraKonto)
		db.Save(&tilKonto)
		//fmt.Print("Penger overført.\nNy saldo for ", fraKonto.Navn, ": ", fraKonto.Saldo, "\nNy saldo for ", tilKonto.Navn, ": ", tilKonto.Saldo, "\n\n")
		printKonto(fraKonto)
		printKonto(tilKonto)
	} else {
		fmt.Print("Det er ikke nok penger på kontoen til å utføre transaksjonen.\n\n")
	}
}

func printKonto(konto Konto) {
	fmt.Print("\n#", konto.Kontonummer,"\nNavn: ", konto.Navn, "\nSaldo: ", konto.Saldo, "\n\n")
}

func whosRich(db * gorm.DB, value float64) []Konto {
	accounts := []Konto{}
	db.Debug().Find(&accounts, "saldo > ?", value)
	return accounts
}

func updateOwner(db *gorm.DB, reader *bufio.Reader) {
	fmt.Print("Kontonummer: ")
	kontonummerRead, _ := reader.ReadString('\n')
	kontonummer, err := strconv.Atoi(strings.Trim(kontonummerRead, "\n"))
	for err != nil {
		fmt.Print("Vennligst oppgi gyldig kontonummer.\nTil konto: ")
		kontonummerRead, _ = reader.ReadString('\n')
		kontonummer, err = strconv.Atoi(strings.Trim(kontonummerRead, "\n"))
	}
	konto := getKonto(db, kontonummer)

	fmt.Print("Nytt navn: ")
	navn, _ := reader.ReadString('\n')

	konto.Navn = strings.Trim(navn, "\n")
	db.Save(&konto)

	//fmt.Print("Kontoens nye navn: ", navn, "\n\n")
	printKonto(konto)
}

func dbInit(db *gorm.DB) {
	db.Debug().DropTableIfExists(&Konto{})
	db.Debug().CreateTable(&Konto{})
}