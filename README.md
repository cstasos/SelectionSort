# Selection Sort - Parallel MPICH

### Αντικείμενο
Η παραλληλοποίηση ενός αλγορίθμου ταξινόμησης με περισσότερους από δύο συμμετέχοντες κόμβους. 
Ο αλγόριθμος Ταξινόμηση με Επιλογή (Selection Sort) θα υλοποιηθεί παράλληλα με την χρήση της MPICH.

### Ταξινόμηση με Επιλογή

Στην ταξινόμηση με επιλογή ο πίνακας διαιρείται σε δύο υπό-πίνακες, τον ταξινομημένο και τον αταξινόμητο. 
Βρίσκει το μικρότερο/μεγαλύτερο (αύξουσα, φθίνουσα) στοιχείο στον αταξινόμητο υπό-πίνακα ανταλλάζοντάς το με το αριστερό 
στοιχείο του υπό-πίνακα και μετακινεί τo όριο (τείχος) του ταξινομημένου υπό-πίνακα κατά ένα στοιχείο δεξιά. Η πολυπλοκότητα του αλγορίθμου είναι O(n2).
