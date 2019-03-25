package pak;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.rmi.Naming;
import java.util.List;

public class main {
    public static void main(String[] args) throws Exception{
        System.out.println("Running!");
        EntityManagerFactory entityManagerFactory = Persistence.
                createEntityManagerFactory("Oving5Persistance");
        EntityManager em = entityManagerFactory.createEntityManager();

        em.getTransaction().begin();

        Konto konto1 = new Konto(0000, 1000, "SindreP");
        Konto konto2 = new Konto(0001, 900, "SindreT");

        em.persist(konto1);
        em.persist(konto2);
        em.flush();

        em.getTransaction().commit();
        em.close();
    }
}
