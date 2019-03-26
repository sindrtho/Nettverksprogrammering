package pak;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.concurrent.TimeUnit;

public class Oppg3 {
    public static void main(String[] args) throws Exception{
        //oppgave 3: 2 klienter, få feil
        (new myThread()).start();
        (new myThread2()).start();
    }
}

class myThread extends Thread{
    @Override
    public void run(){
        System.out.println("Yo! Thread up and running!");
        EntityManagerFactory entityManagerFactory = Persistence.
                createEntityManagerFactory("Oving5Persistance");
        EntityManager em = entityManagerFactory.createEntityManager();

        Konto myKonto = em.find(Konto.class,1);
        em.getTransaction().begin();
        try{
            System.out.println(">>>waitin!");
            TimeUnit.SECONDS.sleep(5);
            System.out.println(">>>Done waitin!");
        }
        catch (Exception e){
            System.out.println(e);
        }
        Konto myKonto2 = em.find(Konto.class,0);
        System.out.println(">>>1: " + myKonto + "\n>>> " + myKonto2);
        myKonto.trekk(100);
        myKonto2.trekk(-100);

        em.flush();
        em.getTransaction().commit();
        em.close();
    }
}

class myThread2 extends Thread{
    @Override
    public void run() {
        System.out.println("Yo! Thread2 up and running!");
        EntityManagerFactory entityManagerFactory = Persistence.
                createEntityManagerFactory("Oving5Persistance");
        EntityManager em = entityManagerFactory.createEntityManager();
        em.getTransaction().begin();

        Konto myKonto = em.find(Konto.class,1);
        Konto myKonto2 = em.find(Konto.class,0);
        System.out.println(">>>2: " + myKonto + "\n>>> " + myKonto2);
        myKonto.trekk(10);
        myKonto2.trekk(-10);

        em.flush();
        em.getTransaction().commit();
        em.close();
    }
}
