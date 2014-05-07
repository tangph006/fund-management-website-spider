using System;
using System.Collections.Generic;
using System.Text;

namespace DecoratorExample
{
    class Program
    {
        static void Main(string[] args)
        {
            Weapon w = new Rifle();
            w.ShowInfo();
            Enhance enhancedWeapon = new Enhance(w);
            enhancedWeapon.EnhanceAmmo();
            enhancedWeapon.ShowInfo();
            Wear wornWeapon = new Wear(w);
            wornWeapon.WearByRate(0.8);
            wornWeapon.ShowInfo();
        }
    }

    abstract class Weapon
    {
        private double ammo;

        public double Ammo
        {
            get { return ammo; }
            set { ammo = value; }
        }

        private double attack;

        public double Attack
        {
            get { return attack; }
            set { attack = value; }
        }

        private double speed;

        public double Speed
        {
            get { return speed; }
            set { speed = value; }
        }

        private string name;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public abstract void ShowInfo();
    }

    class Rifle : Weapon
    {
        public Rifle()
        {
            this.Ammo = 100;
            this.Attack = 10;
            this.Speed = 5;
            this.Name = "Rifle";
        }

        public override void ShowInfo()
        {
            Console.WriteLine(string.Format("ammo\t{0}", Ammo));
            Console.WriteLine(string.Format("attack\t{0}", Attack));
            Console.WriteLine(string.Format("speed\t{0}", Speed));
            Console.WriteLine(string.Format("name\t{0}", Name));
        }
    }

    abstract class Decorator : Weapon
    {
        protected Weapon w;

        public Decorator(Weapon w)
        {
            this.w = w;
        }

        public override void ShowInfo()
        {
            w.ShowInfo();
        }
    }

    class Enhance : Decorator
    {
        public Enhance(Weapon w) : base(w) { }

        public void EnhanceAmmo()
        {
            w.Ammo += 20;
            Console.WriteLine(">>>>>>>>>>>>Enhanced");
        }
    }

    class Wear : Decorator
    {
        public Wear(Weapon w) : base(w) { }
        public void WearByRate(double rate)
        {
            w.Speed = w.Speed * rate;
            w.Attack = w.Attack * rate;
            Console.WriteLine(">>>>>>>>>>>>Worn");
        }
    }
}
