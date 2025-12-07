import java.util.Scanner;
class Solution{
    public static void main (String args[]){
		Scanner sc = new Scanner(System.in);
        int x = sc.nextInt();
		sc.nextLine();
        int y = sc.nextInt();
		sc.nextLine();
        while(y!=0)
        {
            int temp = y;
            y =x%y;
            x = temp;
        }   
        System.out.println("GCD = "+x);
    }
}