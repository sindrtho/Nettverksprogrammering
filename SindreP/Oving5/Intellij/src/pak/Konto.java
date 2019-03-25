package pak;

import java.util.*;
import javax.persistence.*;
import java.io.*;

@Entity
//@NamedQuery(name="finnAntallKonto", query="SELECT COUNT(o) from Konto o")
public class Konto implements Serializable{

    @Id
    //@GeneratedValue(strategy = GenerationType.AUTO)
    private int kontonr;
    private double saldo;
    private String eier;

    public Konto(){}//m ha en konstruktor med tom parameterliste ihht JavaBeans standarden

    public Konto(int kontonr, double saldo, String eier){
        this.kontonr = kontonr;
        this.saldo = saldo;
        this.eier = eier;
    }

    //belop must be positive
    public void trekk(double belop){
        saldo -= belop;
    }

    public int getKontonr() {
        return kontonr;
    }

    public void setKontonr(int kontonr) {
        this.kontonr = kontonr;
    }

    public double getSaldo() {
        return saldo;
    }

    public void setSaldo(double saldo) {
        this.saldo = saldo;
    }

    public String getEier() {
        return eier;
    }

    public void setEier(String eier) {
        this.eier = eier;
    }

    public String toString(){
        return "pak.Konto: kontonr: " + kontonr + " .saldo: " + saldo + " .eier " + eier;
    }
}
