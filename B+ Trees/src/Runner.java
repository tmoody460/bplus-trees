
public class Runner {

	
	public static void main(String[] args){
	
		System.out.println("HELLO WORLD");
		
		BPlusTree b = new BPlusTree();
		
		for(int i = 0; i < 40000; i++) {
			b.insert("record_00" + i + ".dat");
		}
	}
	
}
