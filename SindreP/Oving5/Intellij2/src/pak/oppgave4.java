package pak;

import org.hibernate.LockMode;

import javax.persistence.*;
import java.rmi.Naming;
import java.util.List;

public class oppgave4 {
    public static void main(String[] args) throws Exception{
        System.out.println("Running oppg4!");

        //Husk å sette @version

        (new myThread4()).start();
        (new myThread4()).start();
    }
}

class myThread4 extends Thread{
    @Override
    public void run() {
        System.out.println("Yo! Thread4 up and running!");
        EntityManagerFactory entityManagerFactory = Persistence.
                createEntityManagerFactory("Oving5Persistance");
        EntityManager em = entityManagerFactory.createEntityManager();
        em.getTransaction().begin();

        Konto myKonto = em.find(Konto.class,1);
        Konto myKonto2 = em.find(Konto.class,0);
        System.out.println(">>>" + myKonto + "\n>>>" + myKonto2);
        myKonto.trekk(100);
        myKonto2.trekk(-100);

        em.flush();
        em.getTransaction().commit();
        em.close();
    }
}
