package pak;

import javax.persistence.*;
import java.rmi.Naming;
import java.util.List;

public class main {
    public static void main(String[] args) throws Exception{
        System.out.println("Running Oppgave 2!");
        EntityManagerFactory entityManagerFactory = Persistence.
            createEntityManagerFactory("Oving5Persistance");
        EntityManager em = entityManagerFactory.createEntityManager();

        em.getTransaction().begin();

        Konto konto1 = new Konto(0000, 1000, "SindreP");
        Konto konto2 = new Konto(0001, 500, "SindreT");

        em.persist(konto1);
        em.persist(konto2);

        System.out.println("Sleepin");
        Thread.sleep(1000);

        //Få ut de over beløp
        Query q = em.createQuery("SELECT k FROM Konto k WHERE k.saldo > 501");
        List<Konto> kontos = (List<Konto>)q.getResultList();
        System.out.println(">>> saldo > 500 \n>>>" + kontos.get(0) + "\n>>> siz: " + kontos.size());

        //Endre eier
        Konto myKonto = em.find(Konto.class,1);
        Konto b = em.merge(myKonto);
        b.setEier("SindreP");

        em.flush();
        em.getTransaction().commit();
        em.close();
    }
}
