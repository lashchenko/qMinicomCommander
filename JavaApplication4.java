/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.blogspot.lashchenko;


// general interface
interface Eat {
}

interface EatForHerbivore extends Eat {
    // cow, cat, rabbit ...
}

interface  EatForCarnivore extends Eat {
    // grass, flower, ...
}



abstract class Animal implements Eat{
    boolean use(Eat e) {
        if( this == e ) {
            return false;
        }
        return true;
    }
}

abstract class  Carnivore extends  Animal implements EatForCarnivore {
    
    @Override
    public boolean use(Eat e) {
        if( !super.use(e) || e instanceof EatForHerbivore ) {
            return false;
        }
        return true;
    }
}

abstract class  Herbivore extends  Animal implements EatForCarnivore {
    @Override
    public boolean use(Eat e) {
        if( !super.use(e) || e instanceof EatForCarnivore ) {
            return false;
        }
        return true;
    }
}

abstract class Human extends Animal implements EatForCarnivore{
}

class Man extends Human {
    
}

class Woman extends Human {
    
}


class Lion extends Carnivore {
    
}
class Goat extends Herbivore {
    
}

class Cow extends Herbivore {
    
}

class Grass implements EatForHerbivore {
    
}


/**
 *
 * @author alashchenko
 */
public class JavaApplication4 {
    
    public static void testing(Animal a, Eat e) {
        System.out.println("" + a + " use " + e + " : " + a.use(e));
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        Human man = new Man();
        Human woman = new Woman();
        
        Animal cow = new Cow();
        Animal goat = new Goat();
        Animal lion = new Lion();
        
        Eat grass = new Grass();
        
        testing(cow, grass);
        testing(cow, cow);
        testing(cow, lion);
        
        System.out.println("--------------------------");
        
        testing(lion, lion);
        testing(lion, grass);
        testing(lion, cow);
        testing(lion, goat);
        
        System.out.println("--------------------------");
        
        testing(goat, grass);
        testing(goat, goat);
        testing(goat, lion);
        
        System.out.println("--------------------------");
        
        testing(man, grass);
        testing(man, cow);
        testing(man, goat);
        testing(man, lion);
        testing(man, woman);
        
    }
}
